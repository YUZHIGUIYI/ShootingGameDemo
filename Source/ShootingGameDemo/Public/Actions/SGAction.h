// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/NoExportTypes.h"
#include "SGAction.generated.h"

class UWorld;
class USGActionComponent;

UCLASS(Blueprintable)  // 创建蓝图时可见
class SHOOTINGGAMEDEMO_API USGAction : public UObject
{
	GENERATED_BODY()

protected:

	// 行动Icon
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSoftObjectPtr<UTexture2D> ActionIcon;  // soft reference - 异步加载 - 不会自动加载sgaction蓝图中

	// 当sgaction激活时，标签自动添加到拥有者身上；当sgaction停止时，标签移除
	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer GrantTags;

	// 标签未被添加时，sgaction才能开始
	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer BlockedTags;

	UPROPERTY()
	bool bIsRunning;  // 用于自动执行的定时器事件
	
	UPROPERTY()
	float TimeStarted;

	UPROPERTY()
	USGActionComponent* ActionComp;

protected:

	UFUNCTION(BlueprintCallable, Category = "Action")
	USGActionComponent* GetOwningComponent() const;

public:

	UPROPERTY(EditDefaultsOnly, Category = "Action")
	bool bAutoStart;  // 用于buff或debuff

	UPROPERTY(EditDefaultsOnly, Category = "Action")
	FName ActionName;  // Action nickname to start/stop without a reference to the object
	
public:
	
	void Initialize(USGActionComponent* NewActionComp);

	UFUNCTION(BlueprintCallable, Category = "Action")
	bool IsRunning() const;

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	bool CanStart(AActor* Instigator);

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	void StartAction(AActor* Instigator);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Action")
	void StopAction(AActor* Instigator);

	// 确保可以获取到 GetWorld() - 重写
	UWorld* GetWorld() const override; 
};
