// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "TDSChasingPlayerTask.generated.h"

/**
 * 
 */
UCLASS()
class TDS_API UTDSChasingPlayerTask : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UTDSChasingPlayerTask();

protected:
	UPROPERTY(EditAnywhere)
	float Radius = 30.f;

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector SetKeySelector;

private:
		virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};
