// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "SGTask_DesignatedPatrol.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTINGGAMEDEMO_API USGTask_DesignatedPatrol : public UBTTaskNode
{
	GENERATED_BODY()

protected:

	// 该任务属于ASGShopAssistantCharacter
	UPROPERTY(EditAnywhere, Category = "AI")
	FBlackboardKeySelector MoveToLocationKey;

	int32 RoutePointIndex;

public:

	USGTask_DesignatedPatrol();

	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
