// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actions/SGAction.h"
#include "SGAction_Sprint.generated.h"


UCLASS()
class SHOOTINGGAMEDEMO_API USGAction_Sprint : public USGAction
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SpeedSetting")
	float BounsSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SprintSetting")
	float DelayTime;

	UPROPERTY(EditDefaultsOnly, Category = "SprintSetting")
	float DecreaseSpeed;

	FTimerHandle TimerHandle_SprintDelay;
	
public:

	USGAction_Sprint();

	virtual void StartAction_Implementation(AActor* Instigator) override;

	virtual void StopAction_Implementation(AActor* Instigator) override;

	UFUNCTION()
	void StartSprintDelay_Elapsed(AActor* Instigator);
};
