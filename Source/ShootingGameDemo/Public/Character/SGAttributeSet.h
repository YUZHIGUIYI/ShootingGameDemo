// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "SGAttributeSet.generated.h"

// 定义一个增加各种 Getter和 Setter方法的宏
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * 
 */
UCLASS()
class SHOOTINGGAMEDEMO_API USGAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:

	USGAttributeSet();

public:

	// 生命值
	UPROPERTY(EditDefaultsOnly, Category = "Health")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(USGAttributeSet, Health);

	UPROPERTY(EditDefaultsOnly, Category = "Health")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(USGAttributeSet, MaxHealth);

	// 体力值
	UPROPERTY(EditDefaultsOnly, Category = "Strength")
	FGameplayAttributeData Strength;
	ATTRIBUTE_ACCESSORS(USGAttributeSet, Strength);

	UPROPERTY(EditDefaultsOnly, Category = "Strength")
	FGameplayAttributeData MaxStrength;
	ATTRIBUTE_ACCESSORS(USGAttributeSet, MaxStrength);

	// 伤害
	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	FGameplayAttributeData Attack;
	ATTRIBUTE_ACCESSORS(USGAttributeSet, Attack);

	// 护甲
	UPROPERTY(EditDefaultsOnly, Category = "Armor")
	FGameplayAttributeData Armor;
	ATTRIBUTE_ACCESSORS(USGAttributeSet, Armor);

public:
	
	// 属性修改前回调
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	
	// GE执行后属性回调
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
};
