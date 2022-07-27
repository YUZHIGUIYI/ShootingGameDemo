// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "SGTask_RestoreHealth.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTINGGAMEDEMO_API USGTask_RestoreHealth : public UBTTaskNode
{
	GENERATED_BODY()

	// 重写ExecuteTask函数
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
