// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Types.h"
#include "TDSMeleePointAnimNotify.generated.h"

class USkeletalMeshComponent;

UCLASS()
class TDS_API UTDSMeleePointAnimNotify : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	FOnMeleePointNotify OnMeleePointNotify;

private:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
