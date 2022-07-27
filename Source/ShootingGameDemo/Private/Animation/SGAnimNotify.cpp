// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/SGAnimNotify.h"
#include "Character/SGCharacterBase.h"
#include "Character/SGWeaponComponent.h"

void USGAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	UE_LOG(LogTemp, Log, TEXT("Trigger Animation Notify: SGAnimNotify!"));
	
	// EquipOrMountMontage动画蒙太奇触发通知
	ASGCharacterBase* Character = Cast<ASGCharacterBase>(MeshComp->GetOwner());
	OnSwitchWeaponMessage.Broadcast(Character);
	if (Character != nullptr)
	{
		USGWeaponComponent* WeaponComp = USGWeaponComponent::GetWeaponComponent(Character);
		if (WeaponComp)
		{
			if (Character->bEquipWeapon)
			{
				WeaponComp->EquipWeapon(Character);
			} else
			{
				WeaponComp->MountWeapon(Character);
			}
		}
	}
}
