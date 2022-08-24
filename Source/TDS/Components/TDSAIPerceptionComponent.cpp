// Fill out your copyright notice in the Description page of Project Settings.


#include "TDSAIPerceptionComponent.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AISense_Damage.h"
#include "AIController.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Pawn.h"
#include "TDSAIController.h"
#include "TDSAIBaseCharacter.h"
#include "TDSPlayerState.h"
#include "TDSGameMode.h"

AActor* UTDSAIPerceptionComponent::GetClosestEnemy(ETeamIDType& TeamID)
{	
	const auto AIController = Cast<ATDSAIController>(GetOwner());
	if (!AIController) return nullptr;

	const auto AICharacter = Cast<ATDSAIBaseCharacter>(AIController->GetCharacter());
	if (!AICharacter) return nullptr;

	TArray<AActor*> PerceiveActors;
	const ETeamIDType CurrentTeamID = AICharacter->DefaultTeamID;

	if (SenseClasses.Num() == 0) return nullptr;

	if (CurrentTeamID == ETeamIDType::Neutral)
	{
		GetCurrentlyPerceivedActors(UAISense_Damage::StaticClass(), PerceiveActors);
		if (PerceiveActors.Num() > 0)
		{
			AICharacter->DefaultTeamID = ETeamIDType::Team_2;
			TeamID = ETeamIDType::Team_2;

			const auto AIPlayerState = AIController->GetPlayerState<ATDSPlayerState>();
			if (AIPlayerState)
			{
				AIPlayerState->SetTeamID(AICharacter->DefaultTeamID);
			}

			const auto GameMode = Cast<ATDSGameMode>(GetWorld()->GetAuthGameMode());
			if (GameMode)
			{
				GameMode->SetNumEnemyActors(AIPlayerState->GetTeamID());
			}
		}
		for (const auto& CurrentSenseClass : SenseClasses)
		{
			TeamID = CurrentTeamID;
			GetCurrentlyPerceivedActors(CurrentSenseClass, PerceiveActors);
		}
	}
	else
	{
		for (const auto& CurrentSenseClass : SenseClasses)
		{
			GetCurrentlyPerceivedActors(CurrentSenseClass, PerceiveActors);
			TeamID = CurrentTeamID;
		}

	}
	
	MakeStateEnemy(PerceiveActors, AICharacter, CurrentTeamID);
	
	if (PerceiveActors.Num() == 0) return nullptr;
	AActor* BestPawn = nullptr;
	float BestDistance = MAX_FLT;
	for (const auto& PerceiveActor : PerceiveActors)
	{

		APawn* Player = Cast<APawn>(PerceiveActor);
		if (!Player) return nullptr;

		const auto PlayerState_1 = Player->GetPlayerState<ATDSPlayerState>();
		const auto PlayerState_2 = AICharacter->GetPlayerState<ATDSPlayerState>();
		if (!PlayerState_1 || !PlayerState_2) return nullptr;

		const bool IsEnemySpotted = AreEnemies(PlayerState_1->GetTeamID(), PlayerState_2->GetTeamID());
		const bool bIsNeutral = IsNeutral(PlayerState_1->GetTeamID(), PlayerState_2->GetTeamID());

		if (IsEnemySpotted && PlayerState_1->GetTeamID() != ETeamIDType::Neutral)
		{
			float Distance = (AICharacter->GetActorLocation() - Player->GetActorLocation()).Size();
			if (Distance < BestDistance)
			{
				BestDistance = Distance;
				BestPawn = PerceiveActor;
			}
		}
	}
	return BestPawn;
}


void UTDSAIPerceptionComponent::MakeStateEnemy(TArray<AActor*> EnemyActors, ATDSAIBaseCharacter* AICharacter, ETeamIDType CurrentTeamID)
{
	if (CurrentTeamID == ETeamIDType::Neutral)
	{
		if (EnemyActors.Num() == 0)
		{
			AICharacter->bIsStateOfCalm = false;
		}
		else
		{
			AICharacter->bIsStateOfCalm = true;
	
		}
	}
	else
	{
		AICharacter->bIsStateOfCalm = false;
	}
}

bool UTDSAIPerceptionComponent::AreEnemies(ETeamIDType TeamType, ETeamIDType TeamType2)
{
	return TeamType != TeamType2 ? true : false;
}

bool UTDSAIPerceptionComponent::IsNeutral(ETeamIDType TeamType, ETeamIDType TeamType2) const
{
	return TeamType2 == ETeamIDType::Neutral;
}
