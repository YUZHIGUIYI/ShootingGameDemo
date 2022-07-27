// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/SGAnimInstance.h"
#include "Character/SGCharacterBase.h"

USGAnimInstance::USGAnimInstance()
{
	OwnerCharacter = nullptr;
}

ASGCharacterBase* USGAnimInstance::GetOwnerCharacter()
{
	if (OwnerCharacter == nullptr)
	{
		APawn* OwnerPawn = TryGetPawnOwner();
		OwnerCharacter = OwnerPawn ? Cast<ASGCharacterBase>(OwnerPawn) : nullptr;
	}
	return OwnerCharacter;
}

void USGAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
}

void USGAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
}

void USGAnimInstance::AnimNotify_MountOrSwitchWeapon(UAnimNotify* Notify)
{
	//
	UE_LOG(LogTemp, Log, TEXT("AnimNotify_MountOrSwitchWeapon"));
}
