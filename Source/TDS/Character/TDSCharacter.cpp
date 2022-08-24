#include "TDSCharacter.h"
#include "TDSInteractable.h"
#include "TDSInventoryComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "TDSPlayerStateComponent.h"
#include "TDSProjectileBase.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/World.h"
#include "TDSPlayerState.h"
#include "TDSGameMode.h"
#include "GameFramework/ProjectileMovementComponent.h"

ATDSCharacter::ATDSCharacter(){
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	
	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = false; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Create a camera boom...
	CameraArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraArm"));
	CameraArm->SetupAttachment(RootComponent);
	CameraArm->bUsePawnControlRotation = false; // Don't want arm to rotate when character does
	CameraArm->TargetArmLength = 700.f;
	CameraArm->SetRelativeRotation(FRotator(-65.0f,-10.0f,0.0f));
	CameraArm->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level
	CameraArm->bInheritPitch = false;
	CameraArm->bInheritYaw = false;
	CameraArm->bInheritRoll = false;

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ATDSCharacter::OnPlayerBeginOverlap);
	GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &ATDSCharacter::OnPlayerHit);
	
	// Create a camera...
	CharacterCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CharacterCamera"));
	CharacterCameraComponent->SetupAttachment(CameraArm, USpringArmComponent::SocketName);
	CharacterCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	//Actor components
	PlayerStateInfo = CreateDefaultSubobject<UTDSPlayerStateComponent>(TEXT("PlayerStateInfo"));
	PlayerInventory=CreateDefaultSubobject<UTDSInventoryComponent>(TEXT("Inventory"));
	
	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

void ATDSCharacter::Tick(float DeltaSeconds){
	Super::Tick(DeltaSeconds);
	
	//Cursor drawing
	if (CursorToWorld != nullptr){
		if (APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0)){
			if (PC != nullptr){
				FHitResult TraceHitResult;
				PC->GetHitResultUnderCursor(ECC_Visibility, false, TraceHitResult);
				FVector CursorFV = TraceHitResult.ImpactNormal;
				FRotator CursorR = CursorFV.Rotation();
				CursorToWorld->SetWorldLocation(TraceHitResult.Location);
				CursorToWorld->SetWorldRotation(CursorR);
				if(TraceHitResult.bBlockingHit)
				{
					ATDSInteractable* InteractActor = Cast<ATDSInteractable>(TraceHitResult.GetActor());
					if (InteractActor && !(InteractActor->bItemIsPickuped) && !bObjectPicked)
					{
						bFindInteract = true;
					}
					else bFindInteract = false;
				}
			}
		}
	}
	
	if (GetWorld()->GetTimerManager().IsTimerActive(SkillPickUp) || GetWorld()->GetTimerManager().IsTimerActive(SkillAttack))
	{
		bCanMove = false;
	}
	else
	{
		bCanMove = true;
	}
	
	if(bIsALive && bCanMove)
	{
		CurrentCharSpeed = GetVelocity().Size();
		MeshDirection = GetMesh()->GetAnimInstance()->CalculateDirection(GetVelocity(),GetActorRotation());
		AddMovementInput(FVector(0.0f, -1.0f, 0.0f), AxisX);
		AddMovementInput(FVector(1.0f, 0.0f, 0.0f), AxisY);
		APlayerController* myController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (myController && bIsALive){
			bCanUseSkill = myController->bEnableClickEvents;
			FHitResult ResultHit;
			myController->GetHitResultUnderCursor(ECC_GameTraceChannel1, false, ResultHit);
			float FindRotaterResultYaw = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), ResultHit.Location).Yaw;
			SetActorRotation(FQuat(FRotator(0.0f, FindRotaterResultYaw, 0.0f)));
		}	
	}
	
}

void ATDSCharacter::SetupPlayerInputComponent(UInputComponent* NewInputComponent)
{
	Super::SetupPlayerInputComponent(NewInputComponent);
	
	NewInputComponent->BindAxis(TEXT("MoveForward"), this, &ATDSCharacter::InputAxisX);
	NewInputComponent->BindAxis(TEXT("MoveRight"), this, &ATDSCharacter::InputAxisY);
	NewInputComponent->BindAction(TEXT("Sprint"),IE_Pressed, this, &ATDSCharacter::ActivateSprint);
	NewInputComponent->BindAction(TEXT("Sprint"),IE_Released, this, &ATDSCharacter::DeActivateSprint);
	NewInputComponent->BindAction(TEXT("Fire"),IE_Pressed,this,&ATDSCharacter::FireOn);
	NewInputComponent->BindAction(TEXT("Fire"),IE_Released,this,&ATDSCharacter::FireOff);
	NewInputComponent->BindAction(TEXT("PrevWeapon"), IE_Pressed,this,&ATDSCharacter::PrevWeapon);
	NewInputComponent->BindAction(TEXT("NextWeapon"), IE_Pressed,this,&ATDSCharacter::NextWeapon);
	NewInputComponent->BindAction("PickupTo", IE_Pressed, PlayerInventory, &UTDSInventoryComponent::AddAid);
	NewInputComponent->BindAction("PickupTo", IE_Pressed, this, &ATDSCharacter::SwitchKey);
	NewInputComponent->BindAction("PickupTo", IE_Pressed, this, &ATDSCharacter::PickupToDiary);
	NewInputComponent->BindAction("PickupTo", IE_Released, this, &ATDSCharacter::StopPickupToDiary);
	NewInputComponent->BindAction("UseAid", IE_Pressed, PlayerInventory, &UTDSInventoryComponent::UseAid);
	NewInputComponent->BindAction(TEXT("TargetSkill"), IE_Pressed,this,&ATDSCharacter::SkillBearPickup);
	NewInputComponent->BindAction(TEXT("TargetSkill"), IE_Released,this,&ATDSCharacter::SkillBearDrop);	
}

void ATDSCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (CursorMaterial)
	{
		CursorToWorld = UGameplayStatics::SpawnDecalAtLocation(GetWorld(), CursorMaterial, CursorSize,FVector(0));
	}
}

void ATDSCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	if (!NewController) return;

	const auto MyPlayerState = NewController->GetPlayerState<ATDSPlayerState>();
	if (!MyPlayerState) return;
	
	MyPlayerState->SetTeamID(DefaultTeamID);
	
	const auto GameMode = Cast<ATDSGameMode>(GetWorld()->GetAuthGameMode());
	if (!GameMode) return;
	
	GameMode->SetNumEnemyActors(MyPlayerState->GetTeamID());
}

void ATDSCharacter::InputAxisY(float Value)
{
	AxisY = Value;
}

void ATDSCharacter::InputAxisX(float Value)
{
	AxisX = Value;
}

void ATDSCharacter::ActivateSprint()
{
	PlayerStateInfo->StartSprint();
	GetCharacterMovement()->MaxWalkSpeed = 900.0f;
}

void ATDSCharacter::DeActivateSprint()
{
	PlayerStateInfo->StartStaminaRefresh();
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;
}

UDecalComponent* ATDSCharacter::GetCursorToWorld()
{
	return CursorToWorld;
}

void ATDSCharacter::FireOn()
{
	if(bIsALive && bCanMove)
	{
		if(CurrentWeapon && !bObjectPicked)
		{
			if(!CurrentWeapon->CheckWeaponCanFire() && !CurrentWeapon->GetWorld()->GetTimerManager().IsTimerActive(CurrentWeapon->WeaponFireTimer)) //kweapon key
			{
				PlayAnimMontage(AnimMontageHandleAttack);
                CurrentWeapon->Fire();
			}
			else if (CurrentWeapon->CheckWeaponCanFire() && !CurrentWeapon->GetWorld()->GetTimerManager().IsTimerActive(CurrentWeapon->WeaponFireTimer))
			{
				if(PlayerInventory->CheckCount(CurrentWeapon->WeaponInfo.BulletItemId))
				{
					CurrentWeapon->Fire();
				}
			}
		}
	}
}

void ATDSCharacter::FireOff()
{
	if(CurrentWeapon && bIsALive)
	{
		CurrentWeapon->StopFire();
	}
}

void ATDSCharacter::DecreaseBulletCount()
{
	PlayerInventory->DecreaseCount(CurrentWeapon->WeaponInfo.BulletItemId);
}

void ATDSCharacter::PrevWeapon()
{
	if(bIsALive && !bObjectPicked)
	{
		if(PlayerInventory->WeaponInventory.Num() > 0 && !GetWorld()->GetTimerManager().IsTimerActive(WeaponReloadTimer))
		{			
			if(CurrentWeapon)
			{
				CurrentWeapon->StopFire();
				CurrentWeapon->WeaponInfo.WeaponType=EWeaponType::NoWeapon;
				CurrentWeapon->OnWeaponFire.RemoveDynamic(this, &ATDSCharacter::DecreaseBulletCount);
				CurrentWeapon->Destroy();
			}			
			CurrentWeaponIndex --;
			if(CurrentWeaponIndex<0)
			{
				CurrentWeaponIndex = PlayerInventory->WeaponInventory.Num()-1;
			}
			if(AnimMontageHandleAttack)
				StopAnimMontage(AnimMontageHandleAttack);
			PlayerInventory->OnBulletsEnd.RemoveDynamic(this, &ATDSCharacter::FireOff);			
			CurrentWeapon = SpawnWeapon(CurrentWeaponIndex);
			PlayerInventory->OnBulletsEnd.AddDynamic(this, &ATDSCharacter::FireOff);
			CurrentWeapon->OnWeaponFire.AddDynamic(this, &ATDSCharacter::DecreaseBulletCount);
			OnPlayerWeaponSwitch.Broadcast();
		}
	}
}

void ATDSCharacter::NextWeapon()
{
	if(bIsALive && !bObjectPicked)
	{
		if(PlayerInventory->WeaponInventory.Num()> 0 && !GetWorld()->GetTimerManager().IsTimerActive(WeaponReloadTimer))
		{
			if(CurrentWeapon)
			{
				CurrentWeapon->StopFire();
				CurrentWeapon->WeaponInfo.WeaponType=EWeaponType::NoWeapon;
				CurrentWeapon->OnWeaponFire.RemoveDynamic(this, &ATDSCharacter::DecreaseBulletCount);
				CurrentWeapon->Destroy();
			}			
			CurrentWeaponIndex ++;
			if(CurrentWeaponIndex>=PlayerInventory->WeaponInventory.Num())
			{
				CurrentWeaponIndex = 0;
			}
			if(AnimMontageHandleAttack)
				StopAnimMontage(AnimMontageHandleAttack);
			PlayerInventory->OnBulletsEnd.RemoveDynamic(this, &ATDSCharacter::FireOff);			
			CurrentWeapon = SpawnWeapon(CurrentWeaponIndex);
			PlayerInventory->OnBulletsEnd.AddDynamic(this, &ATDSCharacter::FireOff);
			CurrentWeapon->OnWeaponFire.AddDynamic(this, &ATDSCharacter::DecreaseBulletCount);
			OnPlayerWeaponSwitch.Broadcast();
		}
	}
}

ATDSWeaponBase* ATDSCharacter::SpawnWeapon(int WeaponIndex)
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Owner = GetOwner();
	SpawnParams.Instigator = GetInstigator();
	const FVector Loc(0,0,0);
	const FRotator Rot(0,0,0);
	ATDSWeaponBase* MyWeapon = Cast<ATDSWeaponBase>(GetWorld()->SpawnActor(PlayerInventory->WeaponInventory[WeaponIndex], &Loc, &Rot, SpawnParams));
	MyWeapon->CollisionCapsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	if (MyWeapon && bIsALive)
	{
		FAttachmentTransformRules Rule(EAttachmentRule::SnapToTarget, false);
		MyWeapon->AttachToComponent(GetMesh(), Rule, FName("RightHandSocket"));
	}
	return MyWeapon;
}

void ATDSCharacter::OnPlayerBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(bIsALive)
	{
		ATDSItemBase* BaseActor = Cast<ATDSItemBase>(OtherActor);
		ATDSInteractable* Interactable = Cast<ATDSInteractable>(OtherActor);
		if (BaseActor)
		{
			PlayerInventory->AddItem(BaseActor);
			OtherActor->Destroy();
		}
		else if(Interactable)
		{
			//UE_LOG(LogTemp,Warning,TEXT("LOGIC OVERLAP"));
			if(!bObjectPicked && bPickPressed)
			{
				//UE_LOG(LogTemp,Warning,TEXT("LOGIC ATTACH"));
				Interactable->ItemMesh->SetEnableGravity(false);
				Interactable->ItemMesh->SetSimulatePhysics(false);
				Interactable->ItemMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
				Interactable->ItemHitCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
				Interactable->ItemMovement->ProjectileGravityScale=0;
				bObjectPicked = true;
				FAttachmentTransformRules Rule(EAttachmentRule::SnapToTarget, false);
				Interactable->AttachToComponent(GetMesh(), Rule, FName("LeftHandSocket"));				
			}
		}
	}
}

void ATDSCharacter::OnPlayerHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	
	const ATDSProjectileBase* Bullet = Cast<ATDSProjectileBase>(OtherActor);
	if(Bullet)
	{
		const float BulletDamage = Bullet->ProjectileInfo.ProjectileDamage;
		PlayerStateInfo->SetHealth(-BulletDamage);
	}
}

void ATDSCharacter::SkillBearPickup()
{
	if(bIsALive /*&& PlayerStateInfo->Mana >= 20 && bFindInteract && !bObjectPicked && bCanUseSkill && bPickPressed*/)
	{
		//if(!AnimMontageStartSkill) return;
		PlayAnimMontage(AnimMontageStartSkill);
		float Lenght = AnimMontageStartSkill->CalculateSequenceLength();
		GetWorld()->GetTimerManager().SetTimer(SkillPickUp,Lenght,false,0.0f);
		PlayerStateInfo->StartUseSkill();
		bPickPressed = true;
	}
	//else UE_LOG(LogTemp, Error, TEXT("INTERACT UNAVIABLE"));
}

void ATDSCharacter::SkillBearDrop()
{
	if(bIsALive && bObjectPicked)
	{
		if(!AnimMontageDropSkill) return;
		PlayAnimMontage(AnimMontageDropSkill);
		float Lenght = AnimMontageDropSkill->CalculateSequenceLength();
		GetWorld()->GetTimerManager().SetTimer(SkillAttack,Lenght,false,0.0f);
		PlayerStateInfo->RecoverMana();
	}	
}

void ATDSCharacter::DetachItem()  //call in anim_montage
{
	TArray<AActor*> Actors;
	this->GetAttachedActors(Actors,true);
	for (AActor* a : Actors)
	{
		ATDSInteractable* Interactable = Cast<ATDSInteractable>(a);
		if(Interactable)
		{
			float ItemRotator = GetActorRotation().Yaw + 90.0f;
			FVector ItemForce = GetActorForwardVector();
			Interactable->SetActorRotation(FRotator(0,ItemRotator,0));
			Interactable->ItemHitCollision->SetCollisionResponseToAllChannels(ECR_Overlap);
			Interactable->ItemMovement->bSimulationEnabled = true;
			Interactable->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
			ItemForce.Normalize(0.001);
			Interactable->ItemMovement->Velocity = FVector(ItemForce.X*2000,ItemForce.Y*2000,0);
		}
	}
	bObjectPicked = false;
	bPickPressed = false;
}

void ATDSCharacter::SetMana(float NewMana)
{
	const auto HealthComponent = FindComponentByClass<UTDSPlayerStateComponent>();
	if (!HealthComponent) return;

	HealthComponent->ChangeManaValue(NewMana);
}

void ATDSCharacter::SwitchKey()
{
	if (bIsSwitchEnable && NewTeamID == ETeamIDType::None)
	{
		NewTeamID = ETeamIDType::Team_1;
		//UE_LOG(LogTemp, Warning, TEXT("Success"));
		OnChangeStory.Broadcast();
	}
}

bool ATDSCharacter::ManaIsNotEmpty()
{
	return PlayerStateInfo->GetMana() > 0.0f;
}

void ATDSCharacter::PickupToDiary()
{
	if (bIsCanPickupToDiary)
	{
		bIsPickupToDiary = true;
	}
}

void ATDSCharacter::StopPickupToDiary()
{
	bIsPickupToDiary = false;
}
