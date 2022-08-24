// Fill out your copyright notice in the Description page of Project Settings.


#include "TDSFindEnemyService.h"
#include "TDSAIController.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "TDSAIPerceptionComponent.h"

UTDSFindEnemyService::UTDSFindEnemyService()
{
	NodeName = "FindEnemyService";
}

void UTDSFindEnemyService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	const auto AIController = Cast<ATDSAIController>(OwnerComp.GetAIOwner());
	const auto Blackboard = OwnerComp.GetBlackboardComponent();
	if (!AIController || !Blackboard) return;

	const auto PerceptionComponent = Cast<UTDSAIPerceptionComponent>(AIController->PerceptionComponent);
	if (!PerceptionComponent) return;

	ETeamIDType CurrentStateAffilation = ETeamIDType::None;
	const auto Enemy = PerceptionComponent->GetClosestEnemy(CurrentStateAffilation);
	//if (CurrentStateAffilation == ETeamIDType::Team_1)
	//{
	//	Blackboard->SetValueAsEnum(TeamIDEnumKey.SelectedKeyName, uint8(CurrentStateAffilation));
	//}
	//else
	//{
	//	Blackboard->SetValueAsEnum(TeamIDEnumKey.SelectedKeyName, uint8(CurrentStateAffilation));
	//}
	Blackboard->SetValueAsEnum(TeamIDEnumKey.SelectedKeyName, uint8(CurrentStateAffilation));
	Blackboard->SetValueAsObject(EnemyActorKey.SelectedKeyName, Cast<AActor>(Enemy));


}