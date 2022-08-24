

#include "TDSEnemySpottedTask.h"
#include "BehaviorTree/BlackboardComponent.h"


UTDSEnemySpottedTask::UTDSEnemySpottedTask()
{
	NodeName = "IsEnemySpotted";
}

EBTNodeResult::Type UTDSEnemySpottedTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const auto Blackboard = OwnerComp.GetBlackboardComponent();
	if (!Blackboard) return EBTNodeResult::Failed;

	Blackboard->SetValueAsBool(SetEnemySpottedKey.SelectedKeyName, true);
	return EBTNodeResult::Succeeded;
}