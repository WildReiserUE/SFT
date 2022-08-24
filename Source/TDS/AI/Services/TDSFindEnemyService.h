// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "Types.h"
#include "TDSFindEnemyService.generated.h"

/**
 * 
 */
UCLASS()
class TDS_API UTDSFindEnemyService : public UBTService
{
	GENERATED_BODY()

public:
	UTDSFindEnemyService();

protected:

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector EnemyActorKey;

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector TeamIDEnumKey;
	
};
