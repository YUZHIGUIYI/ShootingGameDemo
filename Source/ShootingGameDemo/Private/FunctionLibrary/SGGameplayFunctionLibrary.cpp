// Fill out your copyright notice in the Description page of Project Settings.


#include "FunctionLibrary/SGGameplayFunctionLibrary.h"
#include "Character/SGActionComponent.h"
#include "Character/SGAttributeComponent.h"
#include "Character/SGWeaponComponent.h"
#include "Player/SGPlayerState.h"
#include "Weapons/SGWeaponBase.h"  // ???

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

bool USGGameplayFunctionLibrary::CheckCreditsWhenPurchasing(AActor* InstigatorActor, int32 Cost)
{
	APawn* PlayerPawn = Cast<APawn>(InstigatorActor);
	if (PlayerPawn)
	{
		ASGPlayerState* PlayerState = PlayerPawn->GetPlayerState<ASGPlayerState>();
		if (PlayerState)
		{
			return PlayerState->RemoveCredits(Cost);
		}
	}
	return false;
}

bool USGGameplayFunctionLibrary::ApplyHealing(AActor* InstigatorActor, float HealingAmount)
{
	USGAttributeComponent* AttributeComp = USGAttributeComponent::GetAttributes(InstigatorActor);
	if (AttributeComp)
	{
		if (!AttributeComp->IsMaxHealth())
		{
			return AttributeComp->ApplyHealthChange(InstigatorActor, HealingAmount);
		}
	}
	return false;
}

bool USGGameplayFunctionLibrary::ApplyClip(AActor* InstigatorActor, FName ClipName, int32 AmmoAmount)
{
	USGWeaponComponent* WeaponComp = USGWeaponComponent::GetWeaponComponent(InstigatorActor);
	if (WeaponComp)
	{
		if (WeaponComp->CurrentWeapon != nullptr)
		{
			FString WeaponName = WeaponComp->CurrentWeapon->WeaponName.ToString();
			if (ClipName.ToString() == WeaponName)
			{
				const int32 OldMaxPrimaryClipAmmo = WeaponComp->CurrentWeapon->MaxPrimaryClipAmmo;
				WeaponComp->CurrentWeapon->MaxPrimaryClipAmmo = OldMaxPrimaryClipAmmo + AmmoAmount;
				
				WeaponComp->OnClipStatusChanged.Broadcast(WeaponComp, WeaponComp->CurrentWeapon->PrimaryClipAmmo,
					WeaponComp->CurrentWeapon->MaxPrimaryClipAmmo);
				
				return true;
			}
		}
	}
	return false;
}
