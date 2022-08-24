// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "TDSMeleeService.generated.h"

/**
 * 
 */
UCLASS()
class TDS_API UTDSMeleeService : public UBTService
{

	GENERATED_BODY()
public:
	UTDSMeleeService();

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector IsCanMeleeKey;
private:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

};
