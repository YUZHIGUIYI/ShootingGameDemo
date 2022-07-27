// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "SGService_CheckHealthState.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTINGGAMEDEMO_API USGService_CheckHealthState : public UBTService
{
	GENERATED_BODY()

public:

	USGService_CheckHealthState();

protected:

	UPROPERTY(EditAnywhere, Category = "AI")
	FBlackboardKeySelector HealthThresholdKey;

	UPROPERTY(EditAnywhere, Category = "AI", meta = (ClampMin = "600.0", ClampMax = "900.0"))
	float Speed;

	UPROPERTY(EditAnywhere, Category = "AI", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float HealthThreshold;

protected:

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
};
