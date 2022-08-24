// Fill out your copyright notice in the Description page of Project Settings.


#include "TDSEscapeEnemyService.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"

const float MaxDistance = 500.0f;

void UTDSEscapeEnemyService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	const auto AIController = OwnerComp.GetAIOwner();
	const auto Blackboard = OwnerComp.GetBlackboardComponent();
	if (!GetWorld() || !AIController || !Blackboard) return;

	const auto Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	const auto AICharacter = AIController->GetPawn();
	if (!AICharacter || !Player) return;

	FVector Direction = AICharacter->GetActorLocation() + Player->GetActorForwardVector() * MaxDistance;
	Blackboard->SetValueAsVector(EscapeLocationKey.SelectedKeyName, Direction);
}