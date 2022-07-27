// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/SGGameplayAbility.h"

FGameplayAbilityInfo USGGameplayAbility::GetAbilityInfo()
{
	// 用于确定冷却时间的游戏效果
	UGameplayEffect* CooldownEffect = GetCooldownGameplayEffect();
	// 用于消耗的游戏效果
	UGameplayEffect* CostEffect = GetCostGameplayEffect();

	float Cooldown = 0.0f;
	float Cost = 0.0f;
	ECostType CostType = ECostType::Strength;

	if (CooldownEffect && CostEffect)
	{
		// 尝试获取传入等级的DurationMagnitude的值并输出到相应的引用变量上
		CooldownEffect->DurationMagnitude.GetStaticMagnitudeIfPossible(1, Cooldown);
		// Modifiers: FGameplayModifierInfo结构体数组用于存储所有数值修改信息
		// 判断是否应用修改数组
		if (CostEffect->Modifiers.Num() > 0)
		{
			FGameplayModifierInfo CostEffectModifierInfo = CostEffect->Modifiers[0];
			CostEffectModifierInfo.ModifierMagnitude.GetStaticMagnitudeIfPossible(1, Cost);

			FGameplayAttribute CostAttribute = CostEffectModifierInfo.Attribute;
			FString CostAttributeString = CostAttribute.AttributeName;  // 查找属性名

			if (CostAttributeString == "Health")
			{
				CostType = ECostType::Health;
			}
			if (CostAttributeString == "Strength")
			{
				CostType = ECostType::Strength;
			}
		}
		return FGameplayAbilityInfo(Cooldown, Cost, CostType, GetClass());
	}

	return FGameplayAbilityInfo();
}


