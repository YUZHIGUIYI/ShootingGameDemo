// Fill out your copyright notice in the Description page of Project Settings.


#include "FunctionLibrary/SGGameplayFunctionLibrary.h"
#include "Character/SGActionComponent.h"
#include "Character/SGAttributeComponent.h"
#include "Character/SGWeaponComponent.h"
#include "GameFramework/Character.h"
#include "Player/SGPlayerState.h"
#include "Weapons/SGWeaponBase.h"  // ???

bool USGGameplayFunctionLibrary::ApplyNormalDamage(AActor* DamageCauser, AActor* TargetActor,
                                                   const FHitResult& ImpactResult, float DirectDamage)
{
	return true;  // 待实现
}

void USGGameplayFunctionLibrary::ShiftOfPerspectives(AActor* PlayerActor, AActor* InteractiveActor, bool bIsVisible)
{
	const ACharacter* PlayerCharacter = Cast<ACharacter>(PlayerActor);
	if (PlayerCharacter)
	{
		APlayerController* PlayerController = Cast<APlayerController>(PlayerCharacter->GetController());
		
		if (PlayerController)
		{
			if (bIsVisible)
			{
				// 退出交互，视角回到玩家身上，设置为游戏模式
				PlayerController->SetViewTargetWithBlend(PlayerActor, 0.5f, EViewTargetBlendFunction::VTBlend_Cubic, 2.0f);
				PlayerController->SetInputMode(FInputModeGameOnly());
				PlayerController->bShowMouseCursor = false;
				UE_LOG(LogTemp, Log, TEXT("Exit Shop!"));
			} else
			{
				// 开始交互，视角转到交互对象上，设置为UI模式
				PlayerController->SetViewTargetWithBlend(InteractiveActor, 0.5f, EViewTargetBlendFunction::VTBlend_Cubic, 2.0f);
				PlayerController->SetInputMode(FInputModeUIOnly());
				PlayerController->bShowMouseCursor = true;
				UE_LOG(LogTemp, Log, TEXT("Enter into Shop!"));
			}
		}
		
		PlayerCharacter->GetMesh()->SetVisibility(bIsVisible);
		const USGWeaponComponent* WeaponComp = USGWeaponComponent::GetWeaponComponent(PlayerActor);
		if (WeaponComp && WeaponComp->CurrentWeapon)
		{
			WeaponComp->CurrentWeapon->GetSkeletalMesh()->SetVisibility(bIsVisible);
		}
	}
}

bool USGGameplayFunctionLibrary::ApplyActionEffect(AActor* InstigatorActor, AActor* TargetActor, float EffectValue)
{
	// InstigatorActor为Effect的施加方，而TargetActor为Effect的受作用方
	USGAttributeComponent* AttributeComp = USGAttributeComponent::GetAttributes(TargetActor);
	if (AttributeComp)
	{
		return AttributeComp->ApplyHealthChange(InstigatorActor, EffectValue);
	}
	return false;
}

bool USGGameplayFunctionLibrary::RequireStrength(AActor* InstigatorActor, float DecreaseSpeed)
{
	USGAttributeComponent* AttributeComp = USGAttributeComponent::GetAttributes(InstigatorActor);
	if (AttributeComp)
	{
		if (AttributeComp->GetStrength() > 1.0f)
		{
			return AttributeComp->ApplyStrengthChange(InstigatorActor, -DecreaseSpeed);
		} else
		{
			AttributeComp->bIsSprinting = false;
		}
	}
	return false;
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
