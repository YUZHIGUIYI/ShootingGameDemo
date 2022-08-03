// Fill out your copyright notice in the Description page of Project Settings.


#include "Actions/SGAction_Sprint.h"
#include "Character/SGCharacterBase.h"
#include "FunctionLibrary/SGGameplayFunctionLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"

USGAction_Sprint::USGAction_Sprint()
{
	BounsSpeed = 650.0f;
	DelayTime = 0.2f;
	DecreaseSpeed = 1.0f;
	
	bAutoStart = false;
	ActionName = "Sprint";
}

void USGAction_Sprint::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);
	
	ASGCharacterBase* PlayerCharacter = Cast<ASGCharacterBase>(Instigator);
	if (PlayerCharacter)
	{
		PlayerCharacter->GetCharacterMovement()->MaxWalkSpeed += BounsSpeed;
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "StartSprintDelay_Elapsed", Instigator);
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_SprintDelay, Delegate, DelayTime, true);
	}
}

void USGAction_Sprint::StartSprintDelay_Elapsed(AActor* Instigator)
{
	if (!USGGameplayFunctionLibrary::RequireStrength(Instigator, DecreaseSpeed))
	{
		StopAction_Implementation(Instigator);  // 强制停止冲刺
	}
}

void USGAction_Sprint::StopAction_Implementation(AActor* Instigator)
{
	if (TimerHandle_SprintDelay.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle_SprintDelay);  // 清除Timer句柄
	}
	
	ASGCharacterBase* PlayerCharacter = Cast<ASGCharacterBase>(Instigator);
	if (PlayerCharacter)
	{
		PlayerCharacter->GetCharacterMovement()->MaxWalkSpeed -= BounsSpeed;
	}
	
	Super::StopAction_Implementation(Instigator);
}


