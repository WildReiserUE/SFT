// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Perception/AIPerceptionComponent.h"
#include "Types.h"
#include "TDSAIPerceptionComponent.generated.h"

class ATDSAIBaseCharacter;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TDS_API UTDSAIPerceptionComponent : public UAIPerceptionComponent
{
	GENERATED_BODY()
	
public:
	AActor* GetClosestEnemy(ETeamIDType& TeamID);

protected:
	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<UAISense>> SenseClasses;
	
	void MakeStateEnemy(TArray<AActor*> EnemyActors, ATDSAIBaseCharacter* AICharacter , ETeamIDType CurrentTeamID);
	void FindAIPawns(TArray<AActor*> AIPawns);

private:
	bool AreEnemies(ETeamIDType TeamType, ETeamIDType TeamType2);
	bool IsNeutral(ETeamIDType TeamType, ETeamIDType TeamType2) const;
};
