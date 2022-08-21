// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SGTask_Patrol.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type USGTask_Patrol::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	APawn* AIPawn = OwnerComp.GetAIOwner()->GetPawn();

	if (AIPawn == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	// 需要加入NavigationSystem模组
	FNavLocation RandomLocation;
	if (UNavigationSystemV1::GetCurrent(GetWorld())->GetRandomPointInNavigableRadius(AIPawn->GetActorLocation(), DesiredRadius, RandomLocation))
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(MoveToRandomLocationKey.SelectedKeyName,RandomLocation);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
