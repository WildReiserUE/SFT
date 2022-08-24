// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Types.h"
#include "TDSBasePickup.generated.h"

class USphereComponent;

UCLASS()
class TDS_API ATDSBasePickup : public AActor
{
	GENERATED_BODY()
	
public:	

	ATDSBasePickup();

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite , Category = "Components")
	USphereComponent* SphereComponent;

	UPROPERTY(EditDefaultsOnly)
	FItemSettings ItemData;
};
