// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "SGAnimNotify.generated.h"

class ASGCharacterBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSwitchWeaponMessage, ASGCharacterBase*, Character);
/**
 * 
 */
UCLASS()
class SHOOTINGGAMEDEMO_API USGAnimNotify : public UAnimNotify
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable)
	FOnSwitchWeaponMessage OnSwitchWeaponMessage;
	
protected:

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
