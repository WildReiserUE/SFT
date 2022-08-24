// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickups/TDSSwitchPickup.h"
#include "Components/SphereComponent.h"
#include "TDSCharacter.h"

ATDSSwitchPickup::ATDSSwitchPickup()
{
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ATDSSwitchPickup::OnBeginOverlap);
	SphereComponent->OnComponentEndOverlap.AddDynamic(this, &ATDSSwitchPickup::OnEndOverlap);
}

void ATDSSwitchPickup::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	const auto Player = Cast<ATDSCharacter>(OtherActor);
	if (!Player) return;

	Player->bIsSwitchEnable = true;
	
}

void ATDSSwitchPickup::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	const auto Player = Cast<ATDSCharacter>(OtherActor);
	if (!Player) return;

	Player->bIsSwitchEnable = false;
}
