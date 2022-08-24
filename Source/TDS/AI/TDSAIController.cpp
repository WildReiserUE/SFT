// Fill out your copyright notice in the Description page of Project Settings.


#include "TDSAIController.h"
#include "TDSAIBaseCharacter.h"
#include "BehaviorTree/BehaviorTree.h"
#include "TDSAIPerceptionComponent.h"
#include "TDSPlayerState.h"
#include "TDSGameMode.h"


ATDSAIController::ATDSAIController()
{
	TDSPerceptionComponent = CreateDefaultSubobject<UTDSAIPerceptionComponent>(TEXT("TDSPerceptionComponent"));
	SetPerceptionComponent(*TDSPerceptionComponent);
	bWantsPlayerState = true;
}

void ATDSAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	const auto AIPawn = Cast<ATDSAIBaseCharacter>(InPawn);
	if (AIPawn)
	{
		RunBehaviorTree(AIPawn->BehaviorTree);
	}

	const auto AIPlayerState = GetPlayerState<ATDSPlayerState>();
	if (AIPlayerState)
	{
		AIPlayerState->SetTeamID(AIPawn->DefaultTeamID);
	}
	
	const auto GameMode = Cast<ATDSGameMode>(GetWorld()->GetAuthGameMode());
	if (GameMode)
	{
		GameMode->SetNumEnemyActors(AIPlayerState->GetTeamID());
	}
}

void ATDSAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	ETeamIDType CurrentTeamID = ETeamIDType::None;
	AActor* FoundActor = TDSPerceptionComponent->GetClosestEnemy(CurrentTeamID);
	if (CurrentTeamID != ETeamIDType::Neutral)
	{
		SetFocus(FoundActor);
	}
}