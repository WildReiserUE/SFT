// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "TDSEnemySpottedTask.generated.h"

/**
 * 
 */
UCLASS()
class TDS_API UTDSEnemySpottedTask : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UTDSEnemySpottedTask();

protected:
	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector SetEnemySpottedKey;

private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};
