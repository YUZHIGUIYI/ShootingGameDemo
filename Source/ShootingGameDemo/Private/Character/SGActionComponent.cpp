// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/SGActionComponent.h"
#include "Actions/SGAction.h"

USGActionComponent::USGActionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void USGActionComponent::BeginPlay()
{
	Super::BeginPlay();

	// 联机状态下需要使用 GetOwner()->HasAuthority() 进行判断
	for (TSubclassOf<USGAction> ActionClass : DefaultActions)
	{
		AddAction(GetOwner(), ActionClass);  // 获取拥有者
	}
}

void USGActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// 调试 - 将拥有的SGAction打印到屏幕上
	for (USGAction* Action : Actions)
	{
		FColor TextColor = Action->IsRunning() ? FColor::Blue : FColor::White;
		FString DebugMsg = GetNameSafe(GetOwner()) + ": " + ActiveGameplayTags.ToStringSimple();
		GEngine->AddOnScreenDebugMessage(-1, 0.0f, TextColor, DebugMsg);  // @Fixme
	}
}

USGActionComponent* USGActionComponent::GetActionComponent(AActor* FromActor)
{
	if (FromActor)
	{
		return Cast<USGActionComponent>(FromActor->GetComponentByClass(USGActionComponent::StaticClass()));
	}
	return nullptr;
}

void USGActionComponent::AddAction(AActor* Instigator, TSubclassOf<USGAction> ActionClass)
{
	// 在此实例化
	if (!ensure(ActionClass))
	{
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("Client attempting to AddAction. [Class: %s]"), *GetNameSafe(ActionClass));  // ??

	// 实例化
	USGAction* NewAction = NewObject<USGAction>(GetOwner(), ActionClass);  // 由 this 改为 owner，即人物
	if (ensure(NewAction))
	{
		NewAction->Initialize(this);  // 见 SGAction 中的 GetOwningComponent() 函数

		Actions.Add(NewAction);

		// 是否可以自动执行
		if (NewAction->bAutoStart && ensure(NewAction->CanStart(Instigator)))
		{
			NewAction->StartAction(Instigator);
		}
	}
}

void USGActionComponent::RemoveAction(USGAction* ActionToRemove)
{
	// 确保不是一个空指针，并且不在运行状态
	if (!ensure(ActionToRemove && !ActionToRemove->IsRunning()))
	{
		return;
	}
	Actions.Remove(ActionToRemove);
}

USGAction* USGActionComponent::GetAction(TSubclassOf<USGAction> ActionClass) const
{
	for (USGAction* Action : Actions)
	{
		if (Action && Action->IsA(ActionClass))
		{
			return Action;  // @???? IsA 匹配类
		}
	}
	return nullptr;
}

bool USGActionComponent::StartActionByName(AActor* Instigator, FName ActionName)
{
	// 统计工具？？？
	for (USGAction* Action : Actions)
	{
		// 检查标签 - 是否可以运行
		if (!Action->CanStart(Instigator))
		{
			FString FailedMsg = FString::Printf(TEXT("Failed to run: %s"), *ActionName.ToString());
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FailedMsg);
			continue;;
		}

		// 提供给 Unreal Insights 的识别书签
		TRACE_BOOKMARK(TEXT("StartAction::%s"), *GetNameSafe(Action));

		// ???
		if (Action && Action->ActionName == ActionName)
		{
			Action->StartAction(Instigator);
			return true;
		}
	}

	return false;
}

bool USGActionComponent::StopActionByName(AActor* Instigator, FName ActionName)
{
	for (USGAction* Action : Actions)
	{
		if (Action && Action->ActionName == ActionName)
		{
			// 只有在已经执行的情况下才能移除标签
			if (Action->IsRunning())
			{
				Action->StopAction(Instigator);
				return true;
			}
		}
	}

	return false;
}

void USGActionComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	// 角色死亡时移除所有的 SGAction
	TArray<USGAction*> ActionCopy = Actions;  // ???
	for (USGAction* Action : ActionCopy)
	{
		if (Action && Action->IsRunning())
		{
			Action->StopAction(GetOwner());
		}
	}
	
	Super::EndPlay(EndPlayReason);
}

