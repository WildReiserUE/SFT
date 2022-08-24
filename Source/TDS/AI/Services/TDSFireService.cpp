// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Services/TDSFireService.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "TDSAIPerceptionComponent.h"
#include "TDSDistantAttackCharacter.h"
#include "TDSPlayerStateComponent.h"

void UTDSFireService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	const auto Controller = OwnerComp.GetAIOwner();
	const auto Blackboard = OwnerComp.GetBlackboardComponent();
	if (!Controller || !Blackboard) return;

	const auto AIPawn = Cast<ATDSDistantAttackCharacter>(Controller->GetPawn());
	if (!AIPawn) return;

	const auto PerceptionComponent = Controller->FindComponentByClass<UTDSAIPerceptionComponent>();
	ETeamIDType CurrentTeamID = ETeamIDType::None;
	const auto Target = PerceptionComponent->GetClosestEnemy(CurrentTeamID);
	const auto TDSPlayerStateComponent = Target ? Target->FindComponentByClass<UTDSPlayerStateComponent>() : nullptr;
	
	bool HasAim = PerceptionComponent && Target && TDSPlayerStateComponent && !TDSPlayerStateComponent->IsDead();

	if (HasAim && CurrentTeamID == ETeamIDType::Team_2)
		AIPawn->StartFire();
	else
		AIPawn->StopFire();
}