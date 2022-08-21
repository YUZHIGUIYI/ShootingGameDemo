// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actions/SGAction.h"
#include "SGAction_Fire.generated.h"

class ASGWeaponBase;
class ASGCharacterBase;
class USGWeaponComponent;

UCLASS()
class SHOOTINGGAMEDEMO_API USGAction_Fire : public USGAction
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, Category = "AnimMontage")
	UAnimMontage* FireMontageNormal;

	UPROPERTY(EditDefaultsOnly, Category = "AnimMontage")
	UAnimMontage* FireMontageViolent;

	UPROPERTY()
	ASGWeaponBase* CurrentWeapon;

	UPROPERTY()
	USGWeaponComponent* WeaponComp;

	bool bCanFire;

	FTimerHandle TimerHandle_FireDelay;

	FCollisionObjectQueryParams ObjParams;

	FCollisionShape Shape;

public:

	USGAction_Fire();

	UFUNCTION()
	void OnDelayFinished();

	UFUNCTION()
	void StartFireDelay_Elapsed(ASGCharacterBase* InstigatorCharacter);

	void StopFireAction(AActor* Instigator);

	void SpawnEmptyAmmo(FActorSpawnParameters& SpawnParams);

	virtual void StartAction_Implementation(AActor* Instigator) override;

	virtual void StopAction_Implementation(AActor* Instigator) override;
	
};
