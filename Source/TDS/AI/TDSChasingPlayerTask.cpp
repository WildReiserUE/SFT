// Fill out your copyright notice in the Description page of Project Settings.


#include "TDSChasingPlayerTask.h"
#include "NavigationSystem.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"

UTDSChasingPlayerTask::UTDSChasingPlayerTask()
{
	NodeName = "ChaseTask";
}

EBTNodeResult::Type UTDSChasingPlayerTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	if(!GetWorld()) return EBTNodeResult::Failed;

	const auto AIController = OwnerComp.GetAIOwner();
	const auto Blackboard = OwnerComp.GetBlackboardComponent();
	const auto PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!AIController || !Blackboard || !PlayerController) return EBTNodeResult::Failed;

	const auto AIPawn = AIController->GetPawn();
	const auto Player = PlayerController->GetCharacter();
	if (!AIPawn || !Player) return EBTNodeResult::Failed;

	const auto NavSys = UNavigationSystemV1::GetCurrent(AIPawn);
	FNavLocation NavLocation;
	FVector Location = Player->GetActorLocation();
	const auto Found = NavSys->GetRandomReachablePointInRadius(Location, Radius, NavLocation);
	if (!Found) return EBTNodeResult::Failed;

	Blackboard->SetValueAsVector(SetKeySelector.SelectedKeyName, NavLocation.Location);
	return EBTNodeResult::Succeeded;
}