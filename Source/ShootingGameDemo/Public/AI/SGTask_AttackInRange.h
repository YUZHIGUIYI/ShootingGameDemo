// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "SGTask_AttackInRange.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTINGGAMEDEMO_API USGTask_AttackInRange : public UBTTaskNode
{
	GENERATED_BODY()

	// 必须返回success或fail
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

public:

	USGTask_AttackInRange();
	
protected:

	// 对应于AI行为树的Blackboard的TargetActor
	UPROPERTY(EditAnywhere, Category = "AI")
	FName TargetActorKey;

	UPROPERTY(EditAnywhere, Category = "AI")
	FName MuzzleSocketName;

	// AI射击时相对于TargetActor的偏移角度 - Yaw 与 Pitch
	UPROPERTY(EditAnywhere, Category = "AI")
	float MaxAmmoOffset;

	UPROPERTY(EditAnywhere, Category = "AI")
	TSubclassOf<AActor> AmmoClass;
};
