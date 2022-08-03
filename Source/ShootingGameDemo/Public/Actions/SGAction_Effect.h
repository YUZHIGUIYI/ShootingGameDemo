// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actions/SGAction.h"
#include "SGAction_Effect.generated.h"

class UParticleSystemComponent;

UCLASS()
class SHOOTINGGAMEDEMO_API USGAction_Effect : public USGAction
{
	GENERATED_BODY()

public:

	USGAction_Effect();

	virtual void StartAction_Implementation(AActor* Instigator) override;

	virtual void StopAction_Implementation(AActor* Instigator) override;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect")
	float Duration;  // 效果持续事件

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect")
	float Period;  // 效果作用周期

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect")
	float EffectValue;

	UPROPERTY(EditDefaultsOnly, Category = "Effect")
	FName AttachPointName;

	UPROPERTY(EditDefaultsOnly, Category = "Effect")
	UParticleSystem* BurningVFX;  // 燃烧特效

	UPROPERTY()
	UParticleSystemComponent* ParticleSystemComp;

	UPROPERTY()
	AActor* TargetActor;

	FTimerHandle TimerHandle_Duration;

	FTimerHandle TimerHandle_Period;

protected:

	UFUNCTION(BlueprintNativeEvent, Category = "Effect")
	void StartPeriodicEffect(AActor* Instigator);

	UFUNCTION(BlueprintCallable, Category = "Effect")
	float GetTimeRemaining() const;
};
