// Fill out your copyright notice in the Description page of Project Settings.


#include "Actions/SGAction_Aiming.h"
#include "Character/SGCharacterBase.h"
#include "Character/SGWeaponComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Player/SGPlayerController.h"
#include "Weapons/SGWeaponBase.h"

USGAction_Aiming::USGAction_Aiming()
{
	ARateTime = 1.0f;
}

void USGAction_Aiming::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);
	
	OwningCharacter = Cast<ASGCharacterBase>(Instigator);
	if (OwningCharacter == nullptr)
	{
		return;
	}

	OwningPlayerController = Cast<ASGPlayerController>(OwningCharacter->GetController());
	
	USGWeaponComponent* WeaponComp = USGWeaponComponent::GetWeaponComponent(OwningCharacter);
	if (WeaponComp)
	{
		CurrentWeapon = WeaponComp->CurrentWeapon;
	}
	
	if (CurrentWeapon)
	{
		if (CurrentWeapon->WeaponType == EWepaonType::Sniper_rifle && OwningPlayerController)
		{
			UGameplayStatics::PlaySound2D(GetWorld(), AimingSoundCue, 2.0f);
			OwningPlayerController->ToggleAimingWidget();
		}
		
		AimingTimelineCallBack.BindUFunction(this, "AimingStart", Instigator);
		FAimingTimeline.AddInterpFloat(AimingTimelineCurve, AimingTimelineCallBack);
		FAimingTimeline.Play();

		FAimingTimeline.TickTimeline(ARateTime);
		
		/*GetWorld()->GetTimerManager().SetTimer(AimingTimerHandle, this, &USGAction_Aiming::TickTimeline, ARateTime,
		true, 0.0f);*/
	}
}

void USGAction_Aiming::AimingStart()
{
	float FloatCurveVal = AimingTimelineCurve->GetFloatValue(FAimingTimeline.GetPlaybackPosition());
	float LerpRet = UKismetMathLibrary::Lerp(300.0f, 90.0f, FloatCurveVal);
	if (CurrentWeapon && CurrentWeapon->WeaponType == EWepaonType::Sniper_rifle)
	{
		LerpRet = UKismetMathLibrary::Lerp(300.0f, 0.0f, FloatCurveVal);
		float LerpRetView = UKismetMathLibrary::Lerp(90.0f, 30.0f, FloatCurveVal);
		OwningCharacter->SetFieldOfView(LerpRetView);
	}
	OwningCharacter->SetSpringArmLength(LerpRet);
}

void USGAction_Aiming::TickTimeline()
{
	if (FAimingTimeline.IsPlaying())
	{
		FAimingTimeline.TickTimeline(ARateTime);
	} else
	{
		GetWorld()->GetTimerManager().ClearTimer(AimingTimerHandle);
	}
}

void USGAction_Aiming::StopAction_Implementation(AActor* Instigator)
{
	if (OwningCharacter)
	{
		OwningCharacter->SetSpringArmLength();
		OwningCharacter->SetFieldOfView();
		if (CurrentWeapon && CurrentWeapon->WeaponType == EWepaonType::Sniper_rifle && OwningPlayerController)
		{
			OwningPlayerController->ToggleAimingWidget();
		}
	}
	Super::StopAction_Implementation(Instigator);
}
