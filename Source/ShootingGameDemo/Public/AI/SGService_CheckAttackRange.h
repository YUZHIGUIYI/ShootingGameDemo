// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "SGService_CheckAttackRange.generated.h"


UCLASS()
class SHOOTINGGAMEDEMO_API USGService_CheckAttackRange : public UBTService
{
	GENERATED_BODY()

public:

	USGService_CheckAttackRange();
	
protected:

	// 对应于AI行为树的Blackboard的TargetActor
	UPROPERTY(EditAnywhere, Category = "AI")
	FName TargetActorKey;

	UPROPERTY(EditAnywhere, Category = "AI")
	FBlackboardKeySelector FoundTargetKey;

	UPROPERTY(EditAnywhere, Category = "AI")
	FBlackboardKeySelector AttackRangeKey;

	UPROPERTY(EditAnywhere, Category = "AI")
	float MaxAttackRange;

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
