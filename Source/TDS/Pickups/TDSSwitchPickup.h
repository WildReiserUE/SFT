// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickups/TDSBasePickup.h"
#include "TDSSwitchPickup.generated.h"

/**
 * 
 */
UCLASS()
class TDS_API ATDSSwitchPickup : public ATDSBasePickup
{
	GENERATED_BODY()

public:
	ATDSSwitchPickup();

private:

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};
