// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "SGTask_Patrol.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTINGGAMEDEMO_API USGTask_Patrol : public UBTTaskNode
{
	GENERATED_BODY()

protected:

	// 该任务尚未被使用
	UPROPERTY(EditAnywhere, Category = "AI")
	FBlackboardKeySelector MoveToRandomLocationKey;

	UPROPERTY(EditAnywhere, Category = "AI")
	float DesiredRadius{1200.0f};
	
public:

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
