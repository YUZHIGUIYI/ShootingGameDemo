// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actions/SGAction.h"
#include "Components/TimelineComponent.h"
#include "SGAction_Aiming.generated.h"

class UCurveFloat;
class ASGCharacterBase;
class ASGPlayerController;
class ASGWeaponBase;
class USoundCue;

UCLASS()
class SHOOTINGGAMEDEMO_API USGAction_Aiming : public USGAction
{
	GENERATED_BODY()

protected:
	
	UPROPERTY()
	ASGCharacterBase* OwningCharacter;

	UPROPERTY()
	ASGPlayerController* OwningPlayerController;

	UPROPERTY(EditDefaultsOnly, Category = "Aiming")
	UCurveFloat* AimingTimelineCurve;

	UPROPERTY(EditDefaultsOnly, Category = "Aiming")
	USoundCue* AimingSoundCue;

	FTimeline FAimingTimeline;
	FOnTimelineFloatStatic AimingTimelineCallBack;

	FTimerHandle AimingTimerHandle;

	UPROPERTY(EditDefaultsOnly, Category = "Aiming")
	float ARateTime;

	UPROPERTY()
	ASGWeaponBase* CurrentWeapon;
	
public:
	
	USGAction_Aiming();

	virtual void StartAction_Implementation(AActor* Instigator) override;

	virtual void StopAction_Implementation(AActor* Instigator) override;

	UFUNCTION()
	void AimingStart();

	UFUNCTION()
	void TickTimeline();
};
