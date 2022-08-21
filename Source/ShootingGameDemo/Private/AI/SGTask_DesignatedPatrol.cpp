// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SGTask_DesignatedPatrol.h"
#include "AIController.h"
#include "AI/SGAIPatrolRoute.h"
#include "AI/SGShopAssistantCharacter.h"
#include "AI/SGSmartObject.h"
#include "BehaviorTree/BlackboardComponent.h"

USGTask_DesignatedPatrol::USGTask_DesignatedPatrol()
{
	bCreateNodeInstance = true;
	NodeName = "Task GetPatrolPoint";  // 减少重新命名的麻烦
}

EBTNodeResult::Type USGTask_DesignatedPatrol::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const ASGShopAssistantCharacter* AICharacter = Cast<ASGShopAssistantCharacter>(OwnerComp.GetAIOwner()->GetPawn());

	if (AICharacter == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	ASGSmartObject* SmartObject = AICharacter->GetSmartObject();

	if (SmartObject == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	ASGAIPatrolRoute* Route = Cast<ASGAIPatrolRoute>(SmartObject);
	if (Route == nullptr || Route->GetRoutePoints().Num() == 0)
	{
		return EBTNodeResult::Failed;
	}

	OwnerComp.GetBlackboardComponent()->SetValueAsVector(MoveToLocationKey.SelectedKeyName, Route->GetRoutePoints()[RoutePointIndex]);
	RoutePointIndex < (Route->GetRoutePoints().Num() - 1) ? ++RoutePointIndex : (RoutePointIndex = 0);

	return EBTNodeResult::Succeeded;
}
