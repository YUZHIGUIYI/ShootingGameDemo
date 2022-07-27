// Fill out your copyright notice in the Description page of Project Settings.


#include "Actions/SGAction.h"
#include "Character/SGActionComponent.h"

USGActionComponent* USGAction::GetOwningComponent() const
{
	//return Cast<USGActionComponent>(GetOuter());
	return ActionComp;
}

void USGAction::Initialize(USGActionComponent* NewActionComp)
{
	ActionComp = NewActionComp;
}

bool USGAction::IsRunning() const
{
	return bIsRunning;
}

bool USGAction::CanStart_Implementation(AActor* Instigator)
{
	if (IsRunning())
	{
		return false;
	}

	USGActionComponent* Comp = GetOwningComponent();

	// 一个动作标签若已执行，还没完成前不能执行其他动作
	if (Comp->ActiveGameplayTags.HasAny(BlockedTags))
	{
		return false;
	}

	return true;
}

void USGAction::StartAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Running: %s"), *GetNameSafe(this));

	// 加入标签
	USGActionComponent* Comp = GetOwningComponent();
	Comp->ActiveGameplayTags.AppendTags(GrantTags);

	bIsRunning = true;

	// 客户端 - Fix later
	TimeStarted = GetWorld()->TimeSeconds;

	// AI角色UI状态更新 - Fix later
	GetOwningComponent()->OnActionStarted.Broadcast(GetOwningComponent(), this);
}

void USGAction::StopAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Stopped: %s"), *GetNameSafe(this));

	// 检查是否已经设置 - 没设置则退出函数
	USGActionComponent* Comp = GetOwningComponent();
	Comp->ActiveGameplayTags.RemoveTags(GrantTags);

	bIsRunning = false;

	// AI角色UI状态更新 - Fix later
	GetOwningComponent()->OnActionStopped.Broadcast(GetOwningComponent(), this);
}

UWorld* USGAction::GetWorld() const
{
	// 见 SGActionComponent 中的 AddAction 函数
	AActor* Actor = Cast<AActor>(GetOuter());
	if (Actor)
	{
		return Actor->GetWorld();
	}
	return nullptr;
}
