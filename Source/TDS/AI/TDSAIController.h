// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Types.h"
#include "TDSAIController.generated.h"

class UTDSAIPerceptionComponent;

UCLASS()
class TDS_API ATDSAIController : public AAIController
{
	GENERATED_BODY()

public:
	ATDSAIController();

	
protected:

	UPROPERTY(EditDefaultsOnly)
	UTDSAIPerceptionComponent* TDSPerceptionComponent;

	virtual void OnPossess(APawn* InPawn) override;
	virtual void Tick(float DeltaTime) override;


};
