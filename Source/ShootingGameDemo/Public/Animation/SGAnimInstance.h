// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "SGAnimInstance.generated.h"

class ASGCharacterBase;

/**
 * 
 */
UCLASS()
class SHOOTINGGAMEDEMO_API USGAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:

	UPROPERTY()
	ASGCharacterBase* OwnerCharacter;
	
protected:

	USGAnimInstance();
	
	ASGCharacterBase* GetOwnerCharacter();

	// 重写初始化动画函数
	void NativeInitializeAnimation() override;

	// 重写更新动画函数
	void NativeUpdateAnimation(float DeltaSeconds) override;

	// 动画通知 - 挂载/更换武器
	UFUNCTION(BlueprintCallable, Category = "AnimNotify")
	void AnimNotify_MountOrSwitchWeapon(UAnimNotify* Notify);
	
};
