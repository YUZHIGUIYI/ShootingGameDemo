// Fill out your copyright notice in the Description page of Project Settings.


#include "Actions/SGAction_Reload.h"
#include "Character/SGCharacterBase.h"
#include "Character/SGWeaponComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Weapons/SGWeaponBase.h"

USGAction_Reload::USGAction_Reload()
{
	ReplaceClipDelay = 1.72f;
}

void USGAction_Reload::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	ASGCharacterBase* InstigatorCharacter = Cast<ASGCharacterBase>(Instigator);
	if (!IsValid(InstigatorCharacter))
	{
		return;
	}

	WeaponComp = USGWeaponComponent::GetWeaponComponent(Instigator);
	CurrentWeapon = WeaponComp->CurrentWeapon;

	if (CurrentWeapon->MaxPrimaryClipAmmo <= 0)
	{
		UE_LOG(LogTemp, Log, TEXT("Clip is empty!"));
		UGameplayStatics::PlaySound2D(GetWorld(), CurrentWeapon->EmptyClipSound);
		return;
	} else if (CurrentWeapon->PrimaryClipAmmo == CurrentWeapon->ClipLoad)
	{
		UE_LOG(LogTemp, Log, TEXT("Clip is full! No need to replace clip!"));
		return;
	} else
	{
		UE_LOG(LogTemp, Log, TEXT("Reload Action Start!"));
		
		InstigatorCharacter->PlayAnimMontage(ReplaceClipMontage);
		CurrentWeapon->GetSkeletalMesh()->PlayAnimation(CurrentWeapon->ReloadSequence, false);

		// 采用定时器的方法实现弹药状态变化与换弹动作的匹配
		// 目前还未找到使用C++实现精准匹配的方法
		// 如果对精确度有要求，可以使用USGWeaponComponent中的换弹函数ReplaceClipCheck和ReplaceClipBegin与蓝图动画通知配合的方式
		FTimerHandle TimerHandle_ReloadDelay;
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "ReloadDelay_Elapsed", Instigator);
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_ReloadDelay, Delegate, ReplaceClipDelay, false);
	}
}

void USGAction_Reload::ReloadDelay_Elapsed(AActor* Instigator)
{
	int32 NeededAmmo = CurrentWeapon->ClipLoad - CurrentWeapon->PrimaryClipAmmo;
	if (CurrentWeapon->MaxPrimaryClipAmmo <= NeededAmmo)
	{
		CurrentWeapon->PrimaryClipAmmo = CurrentWeapon->PrimaryClipAmmo + CurrentWeapon->MaxPrimaryClipAmmo;
		CurrentWeapon->MaxPrimaryClipAmmo = 0;
	} else
	{
		CurrentWeapon->PrimaryClipAmmo = CurrentWeapon->ClipLoad;
		CurrentWeapon->MaxPrimaryClipAmmo = CurrentWeapon->MaxPrimaryClipAmmo - NeededAmmo;
	}
	
	WeaponComp->OnClipStatusChanged.Broadcast(WeaponComp, CurrentWeapon->PrimaryClipAmmo, CurrentWeapon->MaxPrimaryClipAmmo);  // 多播委托
	UE_LOG(LogTemp, Log, TEXT("Reload Completed!"));

	StopAction(Instigator);  // 动作完成，移除GameplayTag
}
