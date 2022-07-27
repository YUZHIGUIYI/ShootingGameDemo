// Fill out your copyright notice in the Description page of Project Settings.


#include "ShootingGameDemo/Public/Character/SGAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffectExtension.h"
#include "ShootingGameDemo/Public/Character/SGCharacterBase.h"

USGAttributeSet::USGAttributeSet() :
	Health(100.0f),
	MaxHealth(100.0f),
	Strength(100.0f),
	MaxStrength(100.0f),
	Attack(0.0f),
	Armor(1.0f)
{
}

void USGAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	//Super::PreAttributeChange(Attribute, NewValue);
	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHealth());
	}
	if (Attribute == GetStrengthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxStrength());
	}
}

void USGAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	// 仅在instant GameplayEffect使Attribute的BaseValue改变时触发
	Super::PostGameplayEffectExecute(Data);

	ASGCharacterBase* TargetCharacter = nullptr;
	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		AActor* TargetActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		TargetCharacter = Cast<ASGCharacterBase>(TargetActor);
	}
}
