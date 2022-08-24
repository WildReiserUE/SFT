// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "TDSChasingService.generated.h"

/**
 * 
 */
UCLASS()
class TDS_API UTDSChasingService : public UBTService
{
	GENERATED_BODY()
	
public:
	UTDSChasingService();

protected:
	UPROPERTY(EditAnywhere)
	float MaxSize = 30.f;

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector SetEnemyKeySelector;

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector SetFloatKeySelector;


private:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

};
