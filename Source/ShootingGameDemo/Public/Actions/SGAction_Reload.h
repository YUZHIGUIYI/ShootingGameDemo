// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actions/SGAction.h"
#include "SGAction_Reload.generated.h"

class USGWeaponComponent;
class ASGWeaponBase;

UCLASS()
class SHOOTINGGAMEDEMO_API USGAction_Reload : public USGAction
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly, Category = "AnimMontage")
	UAnimMontage* ReplaceClipMontage;

	UPROPERTY()
	USGWeaponComponent* WeaponComp;
	
	UPROPERTY()
	ASGWeaponBase* CurrentWeapon;

	UPROPERTY(EditDefaultsOnly, Category = "AnimMontage")
	float ReplaceClipDelay;

public:

	USGAction_Reload();

	virtual void StartAction_Implementation(AActor* Instigator) override;

	UFUNCTION()
	void ReloadDelay_Elapsed(AActor* Instigator); 
};