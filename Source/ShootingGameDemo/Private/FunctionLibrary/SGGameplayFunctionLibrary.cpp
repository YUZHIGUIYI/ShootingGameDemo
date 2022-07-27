// Fill out your copyright notice in the Description page of Project Settings.


#include "FunctionLibrary/SGGameplayFunctionLibrary.h"
#include "Character/SGActionComponent.h"

bool USGGameplayFunctionLibrary::ApplyNormalDamage(AActor* DamageCauser, AActor* TargetActor,
                                                   const FHitResult& ImpactResult, float DirectDamage)
{
	return true;  // 待实现
}

bool USGGameplayFunctionLibrary::CheckGroupByGameplayTag(AActor* DamageCauser, AActor* TargetActor)
{
	USGActionComponent* CauserActionComp = USGActionComponent::GetActionComponent(DamageCauser);
	USGActionComponent* TargetActionComp = USGActionComponent::GetActionComponent(TargetActor);

	if (CauserActionComp && TargetActionComp)
	{
		const FGameplayTag CauserFirstTag = CauserActionComp->ActiveGameplayTags.First();  // 第一个GameplayTag为阵营Tag
		if (TargetActionComp->ActiveGameplayTags.HasTag(CauserFirstTag))
		{
			UE_LOG(LogTemp, Log, TEXT("They are in the same group! Group Name: %s."), *CauserFirstTag.ToString());
			return true;  // 相同阵营
		}
	}
	UE_LOG(LogTemp, Log, TEXT("Not in the same group!!!"));
	return false;
}
