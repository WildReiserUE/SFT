
#include "Components/TDSAIHealthComponent.h"
#include "GameFramework/Character.h"
#include "Math/UnrealMathUtility.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Kismet/GameplayStatics.h"
#include "TDSItemBase.h"
#include "TDSAIBaseCharacter.h"
#include "TDSFireDamageType.h"
#include "Particles/ParticleSystemComponent.h"
#include "TDSProjectileBase.h"
#include "TDSPlayerStateComponent.h"
#include "TDSWeaponBase.h"
#include "GameFramework/PlayerController.h"
#include "TDSProjectileBase.h"


UTDSAIHealthComponent::UTDSAIHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	
	TimerLeft = MaxTimer;
}

void UTDSAIHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	
	Owner = GetOwner();
	if (!Owner) return;
	Health = MaxHealth;
	if (bIsHaveHealth)	
	{
		Owner->OnTakeAnyDamage.AddDynamic(this, &UTDSAIHealthComponent::TakeAnyDamage);
	}
}

void UTDSAIHealthComponent::TakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	if (!GetWorld() || !DamagedActor || !DamageCauser) return;
	if (BloodDecal)
	{
		UGameplayStatics::SpawnDecalAtLocation(GetWorld(), BloodDecal, DecalSize, Owner->GetActorLocation() + FVector(0.0f, 0.0f, 80.0f), DecalRotation, 3.0f);
	}
	GetWorld()->GetTimerManager().ClearTimer(ColorTimerHandle);
	GetWorld()->GetTimerManager().SetTimer(ColorTimerHandle, this, &UTDSAIHealthComponent::ColorChange, 0.1f, true, false);

	SetHealth(-Damage);
	if (Health < MaxHealth && Health > 0.0f && FireFX)
	{
		if (FireSound)
		{
			UGameplayStatics::SpawnSoundAttached(FireSound, Owner->GetRootComponent(), "FireSocket");
		}
		UGameplayStatics::SpawnEmitterAttached(FireFX, Owner->GetRootComponent(), "FireSocket");
	}

	if (IsDead() && bIsEnemy)
	{
		AddHealthEnemy(DamageCauser);
		OnAIDeath.Broadcast();
	}
	else if(IsDead() && !bIsEnemy)
	{
		if (!ExplosionFX) return;
		
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionFX, Owner->GetActorTransform());
		OnAIDeath.Broadcast();
	}

	if (GetWorld() && DamageType->IsA<UTDSFireDamageType>() && bIsEnemy)
	{
		GetWorld()->GetTimerManager().ClearTimer(FireTimerHandle);
		GetWorld()->GetTimerManager().SetTimer(FireTimerHandle, this, &UTDSAIHealthComponent::OnFireDamage, TimerBetween, true, 0.0f);
	}

	ApplySlowAbility(DamageCauser);
	
}

void UTDSAIHealthComponent::AddHealthEnemy(AActor* Target)
{
	if (Target && Target->IsA<ATDSProjectileBase>())
	{
		//const auto WeaponOwner = Cast<APlayerController>(Target->GetOwner());
		const auto Controller = Cast<APlayerController>(Target->GetOwner());
		if (Controller)
		{
			//const APlayerController* Controller = Cast<APlayerController>(WeaponOwner); //WeaponOwner->GetPlayerOwner()
			//if (!Controller) return;

			const auto Player = Controller->GetPawn();
			if (!Player) return;

			const auto HealthComponent = Player->FindComponentByClass<UTDSPlayerStateComponent>();
			if (!HealthComponent) return;

			HealthComponent->RefreshHealth(HealForEnemy);
		}
	}
	else if(Target)
	{
		const auto WeaponOwner = Cast<ATDSWeaponBase>(Target);
		if (WeaponOwner)
		{
			const APlayerController* Controller = Cast<APlayerController>(WeaponOwner->GetPlayerOwner());
			if (!Controller) return;

			const auto Player = Controller->GetPawn();
			if (!Player) return;

			const auto HealthComponent = Player->FindComponentByClass<UTDSPlayerStateComponent>();
			if (!HealthComponent) return;

			HealthComponent->RefreshHealth(HealForEnemy);
		}
	}
}

void UTDSAIHealthComponent::ApplySlowAbility(AActor* Target)
{
	if (!Target) return;
	const auto Projectile = Cast<ATDSProjectileBase>(Target);
	if (!Projectile) return;

	const auto ProjectileInfo = Projectile->GetProjectileInfo();
	if (ProjectileInfo.DefaultProjectileType != EProjectileType::Condensed) return;

	const auto AIOwner = Cast<ATDSAIBaseCharacter>(GetOwner());
	if (!AIOwner) return;

	AIOwner->UpdateVelocityMovement(ProjectileInfo.BulletSlowTargetBy);
}

void UTDSAIHealthComponent::OnFireDamage()
{
	if (!FireEnemyFX) return;
	if (!ParticleSystemComp)
	{
		ParticleSystemComp = UGameplayStatics::SpawnEmitterAttached(FireEnemyFX, GetOwner()->GetRootComponent());
	}

	if (--TimerLeft <= 0 || IsDead())
	{
		TimerLeft = MaxTimer;
		ParticleSystemComp->Deactivate();
		ParticleSystemComp = nullptr;
		GetWorld()->GetTimerManager().ClearTimer(FireTimerHandle);
	}
	SetHealth(-BaseDamage);
	
}

void UTDSAIHealthComponent::ColorChange()
{
	if (Counter <= 0)
	{
		GetWorld()->GetTimerManager().ClearTimer(ColorTimerHandle);
		Counter = 4;
	}

	const auto Character = Cast<ACharacter>(Owner);
	if (!Character || !Character->GetMesh()) return;

	const auto Mesh = Character->GetMesh();
	const auto MID = Mesh->CreateAndSetMaterialInstanceDynamic(0);
	if (!MID) return;

	if (bIsColorChange)
	{
		MID->SetVectorParameterValue("Color", FirstColor);
		bIsColorChange = false;
	}
	else
	{
		MID->SetVectorParameterValue("Color", SecondColor);
		bIsColorChange = true;
	}
	--Counter;
}

void UTDSAIHealthComponent::SetHealth(float CurrentHealth)
{
	Health = FMath::Clamp(Health + CurrentHealth, 0.0f, MaxHealth);
}

bool UTDSAIHealthComponent::IsDead() const 
{
	//UE_LOG(LogTemp, Error, TEXT("Health = %f"), Health);
	return Health == 0.0f;
}