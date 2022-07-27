// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "SGGameplayAbility.generated.h"

class USGGameplayAbility;

UENUM(BlueprintType)
enum class ECostType : uint8
{
	Health,
	Strength
};

USTRUCT(BlueprintType)
struct FGameplayAbilityInfo
{
	GENERATED_BODY()

	// 冷却
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameplayAbility")
	float CoolDown;

	// 消耗量
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameplayAbility")
	int Cost;

	// 消耗类型
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameplayAbility")
	ECostType CostType;

	// 返回类自身用于做相关信息的获取
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameplayAbility")
	TSubclassOf<USGGameplayAbility> AbilityClass;

	FGameplayAbilityInfo() : CoolDown(0.0f), Cost(0), CostType(ECostType::Strength), AbilityClass(nullptr) {}

	FGameplayAbilityInfo(float CoolDown, int Cost, ECostType CostType, TSubclassOf<USGGameplayAbility> AbilityClass) :
		CoolDown(CoolDown), Cost(Cost), CostType(CostType), AbilityClass(AbilityClass) {}
};

UCLASS()
class SHOOTINGGAMEDEMO_API USGGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "GameplayAbility")
	FGameplayAbilityInfo GetAbilityInfo();
};
