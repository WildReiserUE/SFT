// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "Types.h"
#include "TDSFireService.generated.h"


UCLASS()
class TDS_API UTDSFireService : public UBTService
{
	GENERATED_BODY()
	
private:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
