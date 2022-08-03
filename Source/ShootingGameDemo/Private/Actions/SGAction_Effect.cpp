// Fill out your copyright notice in the Description page of Project Settings.


#include "Actions/SGAction_Effect.h"
#include "Character/SGActionComponent.h"
#include "FunctionLibrary/SGGameplayFunctionLibrary.h"
#include "GameFramework/Character.h"
#include "GameFramework/GameStateBase.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

USGAction_Effect::USGAction_Effect()
{
	bAutoStart = true;
	ActionName = "BurningEffect";

	EffectValue = 6.0f;
	Duration = 4.0f;
	Period = 1.0f;
}

void USGAction_Effect::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	if (ActionComp)
	{
		TargetActor = ActionComp->GetOwner();
		if (TargetActor)
		{
			ACharacter* AffectedCharacter = Cast<ACharacter>(TargetActor);
			if (BurningVFX && AffectedCharacter)
			{
				ParticleSystemComp = UGameplayStatics::SpawnEmitterAttached(BurningVFX, AffectedCharacter->GetMesh(), AttachPointName, 
				AffectedCharacter->GetActorLocation(), FRotator::ZeroRotator, FVector(1), EAttachLocation::KeepWorldPosition, true);
			}
		}
	}
	
	if (Duration > 0.0f)
	{
		FTimerDelegate DurationDelegate;
		DurationDelegate.BindUFunction(this, "StopAction", Instigator);

		GetWorld()->GetTimerManager().SetTimer(TimerHandle_Duration, DurationDelegate, Duration, false);
	}

	if (Period > 0.0f)
	{
		FTimerDelegate PeriodDelegate;
		PeriodDelegate.BindUFunction(this, "StartPeriodicEffect", Instigator);

		GetWorld()->GetTimerManager().SetTimer(TimerHandle_Period, PeriodDelegate, Period, true);
	}
}

void USGAction_Effect::StopAction_Implementation(AActor* Instigator)
{
	// 确保Period计时器句柄归零前Effect仍能执行
	if (GetWorld()->GetTimerManager().GetTimerRemaining(TimerHandle_Period) < KINDA_SMALL_NUMBER)
	{
		StartPeriodicEffect(Instigator);
	}

	if (ParticleSystemComp)
	{
		ParticleSystemComp->DestroyComponent(true);  // 销毁燃烧特效
		UE_LOG(LogTemp, Warning, TEXT("Destroy ParticleSystemComponent!"));
	}
	
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle_Duration);
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle_Period);
	
	Super::StopAction_Implementation(Instigator);

	if (ActionComp)
	{
		ActionComp->RemoveAction(this);  // 将Effect从ActionComponent中移除，Effect不再生效
		UE_LOG(LogTemp, Log, TEXT("Remove Action Effect."));
	}
}

void USGAction_Effect::StartPeriodicEffect_Implementation(AActor* Instigator)
{
	if (TargetActor)
	{
		USGGameplayFunctionLibrary::ApplyActionEffect(Instigator, TargetActor, -EffectValue);
		UE_LOG(LogTemp, Log, TEXT("Action Effect Works."));
	}
}

float USGAction_Effect::GetTimeRemaining() const
{
	// 获取剩余时间
	AGameStateBase* GameState = GetWorld()->GetGameState<AGameStateBase>();
	if (GameState)
	{
		float EndTime = TimeStarted + Duration;
		return EndTime - GameState->GetServerWorldTimeSeconds();
	}

	return Duration;
}
