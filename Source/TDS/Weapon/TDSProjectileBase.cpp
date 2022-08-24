// Fill out your copyright notice in the Description page of Project Settings.


#include "TDSProjectileBase.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/Character.h"
#include "TDSAIBaseCharacter.h"
#include "Perception/AISense_Damage.h"
#include "TDSWeaponBase.h"


// Sets default values
ATDSProjectileBase::ATDSProjectileBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	SetRootComponent(ProjectileMesh);
	ProjectileMesh->OnComponentHit.AddDynamic(this, &ATDSProjectileBase::ProjectileHit);
	
	ProjectileFX = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("MoveEffect"));
	ProjectileFX->SetupAttachment(RootComponent);
	ProjectileInfo.HitFX = CreateDefaultSubobject<UParticleSystem>(TEXT("HitFX"));
	
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComponent"));
	ProjectileMovement->UpdatedComponent = RootComponent;
	ProjectileMovement->InitialSpeed=0.0f;
	ProjectileMovement->MaxSpeed=0.0f;
	ProjectileMovement->bRotationFollowsVelocity=true;
	ProjectileMovement->bShouldBounce=false;
}

// Called every frame
void ATDSProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if(GetWorld()->GetTimerManager().IsTimerActive(BulletDestroyTimer))
	{
		TimerToDestroy();
	}
}

void ATDSProjectileBase::ProjectileHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if(ProjectileMovement->bShouldBounce && !(GetWorld()->GetTimerManager().IsTimerActive(BulletDestroyTimer)))
	{
		GetWorld()->GetTimerManager().SetTimer(BulletDestroyTimer,this, &ATDSProjectileBase::TimerToDestroy, ProjectileInfo.TimeToDestroy, false, -1);		
	}
	else if(!ProjectileMovement->bShouldBounce)
	{
		if(ProjectileInfo.HitFX)
			UGameplayStatics::SpawnEmitterAtLocation(this, ProjectileInfo.HitFX, Hit.Location, FRotator::ZeroRotator, FVector (1),true,EPSCPoolMethod::None,true);
		UGameplayStatics::ApplyDamage(OtherActor, ProjectileInfo.ProjectileDamage, GetWorld()->GetFirstPlayerController(), this, {});
		if (OtherActor && OtherActor->IsA<ATDSAIBaseCharacter>())
		{
			//const auto Weapon = Cast<ATDSWeaponBase>(GetOwner());
			AController* Controller = Cast<AController>(GetOwner());
			if (Controller)
			{
				/*AController* Controller = Cast<AController>(Weapon->GetPlayerOwner());
				if (!Controller) return;*/

				APawn* Player = Controller->GetPawn();
				if (!Player) return;

				UAISense_Damage::ReportDamageEvent(GetWorld(), OtherActor, Player, 1.0f, Player->GetActorLocation(), Player->GetActorLocation());
			}
		}
		Destroy();
	}
	
	if (Hit.bBlockingHit && OtherActor && OtherActor->IsA<ATDSItemBase>())
	{
		MoveObject(OtherComp);
	}
	SpawnBloodFX(Hit);

}

void ATDSProjectileBase::TimerToDestroy()
{
	float Time = GetWorld()->GetTimerManager().GetTimerElapsed(BulletDestroyTimer);
	if(Time >= ProjectileInfo.TimeToDestroy)
	{
		Destroy();
	}
}

void ATDSProjectileBase::MoveObject(UPrimitiveComponent* MeshObject)
{
	if (!MeshObject) return;

	const FVector DirectionImpulse = GetActorForwardVector() * ProjectileInfo.Force * MeshObject->GetMass();
	MeshObject->AddImpulse(DirectionImpulse);
}

void ATDSProjectileBase::SpawnBloodFX(const FHitResult& Hit)
{
	if (!GetWorld() || !ProjectileInfo.BloodFX || !Hit.bBlockingHit || !Hit.GetActor() || !Hit.Actor->IsA<ACharacter>()) return;
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ProjectileInfo.BloodFX, Hit.ImpactPoint);
}