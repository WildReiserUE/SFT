// Fill out your copyright notice in the Description page of Project Settings.


#include "Environment/TDSBaseEnvironment.h"
#include "TDSAIHealthComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

ATDSBaseEnvironment::ATDSBaseEnvironment()
{
	PrimaryActorTick.bCanEverTick = false;
	StaticMeshObject = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EnvironmentMesh"));
	SetRootComponent(StaticMeshObject);

	AIHealthComponent = CreateDefaultSubobject<UTDSAIHealthComponent>(TEXT("HealthComponent"));;
}


void ATDSBaseEnvironment::BeginPlay()
{
	Super::BeginPlay();

	check(AIHealthComponent);
	AIHealthComponent->OnAIDeath.AddUObject(this, &ATDSBaseEnvironment::OnDead);
}

void ATDSBaseEnvironment::OnDead()
{
	if (!GetWorld() || !SoundExplosion) return;
	if (!bIsActiveTimerDamage)
	{
		UGameplayStatics::ApplyRadialDamage(GetWorld(), Damage, GetActorLocation(), DamageRadius, {}, {}, this);
		DrawDebugSphere(GetWorld(), GetActorLocation(), DamageRadius, 8, FColor::Red, false, 3.0f, 0, 3.0f);
		Destroy();
	}
	else
	{
		if (!FireFX) return;
		UGameplayStatics::SpawnEmitterAttached(FireFX, StaticMeshObject);
		StaticMeshObject->SetHiddenInGame(true);
		StaticMeshObject->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		GetWorld()->GetTimerManager().SetTimer(ExplosionTimerHandle, this, &ATDSBaseEnvironment::TimerDamage, TimerBetween, true, 0.0f);
	}
	UGameplayStatics::SpawnSoundAtLocation(GetWorld(), SoundExplosion, GetActorLocation());
}

void ATDSBaseEnvironment::TimerDamage()
{
	if (--TimerLeft <= 0)
	{
		GetWorld()->GetTimerManager().ClearTimer(ExplosionTimerHandle);
		Destroy();
	}
	UGameplayStatics::ApplyRadialDamage(GetWorld(), DamageEverySecond, GetActorLocation(), DamageRadiusFire, DamageTypeClass, {}, this);
	DrawDebugSphere(GetWorld(), GetActorLocation(), DamageRadiusFire, 8, FColor::Yellow, false, 3.0f, 0, 3.0f);

}

