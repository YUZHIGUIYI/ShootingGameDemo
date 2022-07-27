// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "SGActionComponent.generated.h"

class USGAction;

// 事件分发 - 用于AI角色的UI状态更新
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnActionStateChanged, USGActionComponent*, OwningComp, USGAction*, Action);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHOOTINGGAMEDEMO_API USGActionComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	// 加入游戏标签，要求事先在Build.cs文件中加入GameplayTags
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameplayTags")
	FGameplayTagContainer ActiveGameplayTags;  // 必须在此加入头文件，否则无法编译
	
public:

	UFUNCTION(BlueprintCallable, Category = "Actions")
	void AddAction(AActor* Instigator, TSubclassOf<USGAction> ActionClass);

	UFUNCTION(BlueprintCallable, Category = "Actions")
	void RemoveAction(USGAction* ActionToRemove);

	// 返回第一个与提供的类匹配的USGAction
	UFUNCTION(BlueprintCallable, Category = "Actions")
	USGAction* GetAction(TSubclassOf<USGAction> ActionClass) const;

	UFUNCTION(BlueprintCallable, Category = "Actions")
	bool StartActionByName(AActor* Instigator, FName ActionName);

	UFUNCTION(BlueprintCallable, Category = "Actions")
	bool StopActionByName(AActor* Instigator, FName ActionName);

	UFUNCTION(BlueprintCallable, Category = "Actions")
	static USGActionComponent* GetActionComponent(AActor* FromActor);
	
	USGActionComponent();

public:

	// 用于AI角色的UI更新
	UPROPERTY(BlueprintAssignable)
	FOnActionStateChanged OnActionStarted;

	UPROPERTY(BlueprintAssignable)
	FOnActionStateChanged OnActionStopped;
	
protected:

	// 游戏开始时的SGAction
	UPROPERTY(EditDefaultsOnly, Category = "Actions")
	TArray<TSubclassOf<USGAction>> DefaultActions;

	// SGAction列表
	UPROPERTY(BlueprintReadOnly)
	TArray<USGAction*> Actions;

protected:
	
	// 游戏开始时调用 - 初始化
	virtual void BeginPlay() override;

	// 游戏结束时清除
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
};
