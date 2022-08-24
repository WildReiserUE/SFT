// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Types.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Actor.h"
#include "TDSItemBase.generated.h"

UCLASS()
class TDS_API ATDSItemBase : public AActor
{
	GENERATED_BODY()
	
public:	

	ATDSItemBase();
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ItemSettings")
	FItemSettings ItemInfo;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ItemInfo", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* StaticMeshItem = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ItemInfo", meta = (AllowPrivateAccess = "true"))
	UCapsuleComponent* CollisionCapsule;
	bool aAllow;
};
