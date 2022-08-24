// Fill out your copyright notice in the Description page of Project Settings.


#include "TDSChasingService.h"
#include "NavigationSystem.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "AIController.h"
#include "TDSAIPerceptionComponent.h"

UTDSChasingService::UTDSChasingService()
{
	NodeName = "EnemyChase";
}

void UTDSChasingService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	if (!GetWorld()) return;

	const auto AIController = OwnerComp.GetAIOwner();
	const auto Blackboard = OwnerComp.GetBlackboardComponent();
	const auto PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!AIController || !Blackboard || !PlayerController) return;

	const auto AIPawn = AIController->GetPawn();
	const auto Player = PlayerController->GetCharacter();
	if (!AIPawn || !Player) return;

	const FVector Location = Player->GetActorLocation();
	const float Size = (Location - AIPawn->GetActorLocation()).Size();

	//UE_LOG(LogTemp, Error, TEXT("Size = %f"), Size);
	Blackboard->SetValueAsObject(SetEnemyKeySelector.SelectedKeyName, Player);
	Blackboard->SetValueAsFloat(SetFloatKeySelector.SelectedKeyName, Size);
}