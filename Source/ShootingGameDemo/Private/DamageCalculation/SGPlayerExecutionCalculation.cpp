// Fill out your copyright notice in the Description page of Project Settings.


#include "ShootingGameDemo/Public/DamageCalculation/SGPlayerExecutionCalculation.h"

#include "ShootingGameDemo/Public/Character/SGAttributeSet.h"

USGPlayerExecutionCalculation::USGPlayerExecutionCalculation()
{
	// 将宏定义方法声明出来的值进行定义
	DEFINE_ATTRIBUTE_CAPTUREDEF(USGAttributeSet, Health, Target, true);
	DEFINE_ATTRIBUTE_CAPTUREDEF(USGAttributeSet, Attack, Target, true);
	DEFINE_ATTRIBUTE_CAPTUREDEF(USGAttributeSet, Armor, Target, true);
	

	// 添加进捕捉列表
	RelevantAttributesToCapture.Add(HealthDef);
	RelevantAttributesToCapture.Add(AttackDef);
	RelevantAttributesToCapture.Add(ArmorDef);
}

void USGPlayerExecutionCalculation::Execute_Implementation(
	const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	float AttackMagnitude, ArmorMagnitude = 0.0f;

	// 尝试计算捕捉的属性大小
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(AttackDef, FAggregatorEvaluateParameters(),
					AttackMagnitude);
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(ArmorDef, FAggregatorEvaluateParameters(),
					ArmorMagnitude);

	// 计算最终伤害值
	float FinalDamage = FMath::Clamp(AttackMagnitude - ArmorMagnitude, 0.0f, AttackMagnitude - ArmorMagnitude);
	OutExecutionOutput.AddOutputModifier(
		FGameplayModifierEvaluatedData(HealthProperty, EGameplayModOp::Additive, -FinalDamage));
}
