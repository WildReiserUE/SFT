#include "TDSInteractable.h"
#include "TDSAIBaseCharacter.h"
#include "TDSCharacter.h"
#include "TDSProjectileBase.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
ATDSInteractable::ATDSInteractable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	RootComponent = ItemMesh;
	ItemMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	ItemMesh->SetSimulatePhysics(true);
	ItemMesh->OnComponentHit.AddDynamic(this, &ATDSInteractable::OnHit);
	ItemMesh->OnClicked.AddDynamic(this, &ATDSInteractable::OnClicked);
	
	ItemHitCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("HitBoxCollision"));
	ItemHitCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	ItemHitCollision->SetupAttachment(ItemMesh);
	ItemHitCollision->OnComponentBeginOverlap.AddDynamic(this, &ATDSInteractable::ATDSInteractable::OnItemBeginOverlap);
	
	ItemMovement=CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement"));	
	ItemMovement->bShouldBounce = true;
	ItemMovement->ProjectileGravityScale = 0;
	ItemMovement->InitialSpeed = 0.0f;
	ItemMovement->MaxSpeed = 6000.0f;
}
void ATDSInteractable::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{	

}

void ATDSInteractable::OnItemBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Damage>0)
	{
		const ATDSCharacter* Player = Cast<ATDSCharacter>(OtherActor);
		const ATDSProjectileBase* Projectile = Cast<ATDSProjectileBase>(OtherActor);
		const ATDSAIBaseCharacter* Enemy = Cast<ATDSAIBaseCharacter>(OtherActor);
		const ATDSInteractable* Self = Cast<ATDSInteractable>(OtherActor);
		const UStaticMeshComponent* StaticMesh = Cast<UStaticMeshComponent>(OtherComp);
		if(Player || Self || Projectile) return;
		
		if(Enemy)
		{			
			if(bIsFirstHit)
			{
				UE_LOG(LogTemp, Error, TEXT("FLY ITEM INTERACT --- ENEMY"));
				ItemParamsHit();
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitEnemyFX, GetActorLocation());
				UGameplayStatics::PlaySoundAtLocation(GetWorld(),HitEnemy, GetActorLocation());
				UGameplayStatics::ApplyDamage(OtherActor, Damage, GetWorld()->GetFirstPlayerController(),GetWorld()->GetFirstPlayerController(),UDamageType::StaticClass());			
				//UE_LOG(LogTemp, Error, TEXT("FLY ITEM FIRST HIT"));				
			}
			UGameplayStatics::PlaySoundAtLocation(GetWorld(),HitStatic, GetActorLocation());
		}
		else if(StaticMesh)
		{
			//UE_LOG(LogTemp, Error, TEXT("FLY ITEM INTERACT --- StaticMesh"));
			//UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitEnemyFX, GetActorLocation());
			UGameplayStatics::PlaySoundAtLocation(GetWorld(),HitStatic, GetActorLocation());
			ItemParamsHit();
		}
	}
}

void ATDSInteractable::OnClicked(UPrimitiveComponent* Component, FKey FKey)
{
	if(!bItemIsPickuped)
	{
		bIsFirstHit=true;
		if(SoundMove)
			UGameplayStatics::PlaySoundAtLocation(GetWorld(),SoundMove, GetActorLocation());
		//UE_LOG(LogTemp, Error, TEXT("ITEM --- CLICKED"));
		ItemMesh->SetCollisionResponseToAllChannels(ECR_Overlap);
		ItemMesh->SetSimulatePhysics(false);
		ItemMovement->bSimulationEnabled = false;
		ItemMovement->ProjectileGravityScale = 0;
		bItemIsPickuped = true;		
	}	
}

void ATDSInteractable::ItemParamsDefault()
{	
	ItemMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	ItemMesh->SetSimulatePhysics(true);
	bItemIsPickuped = false;
	ItemMovement->bShouldBounce = true;
	ItemMovement->ProjectileGravityScale = 0;
}

void ATDSInteractable::ItemParamsHit()
{
	bIsFirstHit = false;
	ItemMesh->SetSimulatePhysics(true);
	ItemMesh->SetCollisionResponseToAllChannels(ECR_Block);
	bItemIsPickuped = false;
	ItemMesh->SetEnableGravity(true);	
	ItemMovement->ProjectileGravityScale=9.8f;
}
