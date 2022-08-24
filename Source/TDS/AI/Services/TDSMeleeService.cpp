// Fill out your copyright notice in the Description page of Project Settings.


#include "TDSMeleeService.h"
#include "TDSAIMeleeCharacter.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UTDSMeleeService::UTDSMeleeService()
{
	NodeName = "MeleeService";
}

void UTDSMeleeService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	const auto Controller = OwnerComp.GetAIOwner();
	const auto Blackboard = OwnerComp.GetBlackboardComponent();
	if (!Controller || !Blackboard) return;

	const auto AIPawn = Cast<ATDSAIMeleeCharacter>(Controller->GetPawn());
	if (!AIPawn) return;

	int32 IsMelee = AIPawn->IsCanMelee();
	Blackboard->SetValueAsInt(IsCanMeleeKey.SelectedKeyName, IsMelee);
}