// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SGPlayerController.generated.h"

// 更新状态
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPawnChanged, APawn*, NewPawn);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerStateChanged, APlayerState*, NewPlayerState);

UCLASS()
class SHOOTINGGAMEDEMO_API ASGPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:

	// 暂停菜单class
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> PauseMenuClass;
	// 暂停菜单实例
	UPROPERTY()
	UUserWidget* PauseMenuInstance;
	// 背包菜单class
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> BackpackUIClass;
	// 背包菜单实例
	UPROPERTY()
	UUserWidget* BackpackUIInstance;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> MissionTipsClass;  // 任务提示

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> MissionSettlementClass;  // 任务结算

	UPROPERTY()
	UUserWidget* MissionUIInstance;  // 任务UI实例
	
	UPROPERTY(BlueprintAssignable)
	FOnPawnChanged OnPawnChanged;

	UPROPERTY(BlueprintAssignable)
	FOnPlayerStateChanged OnPlayerStateReceived;  // 解决客户端在加入游戏时无法尽早获得PlayerState的问题

protected:

	virtual void SetupInputComponent() override;  // 重写输入事件绑定按键

	UFUNCTION(BlueprintCallable)
	void TogglePauseMenu();  // ESC键切换暂停菜单

	UFUNCTION(BlueprintCallable)
	void ToggleBackpackWidget();  // B键切换背包组件

	UFUNCTION(BlueprintCallable)
	void ToggleMissionTipsWidget();  // 任务提示

	// 重写 - 玩家进入游戏或玩家死亡复活时调用
	virtual void SetPawn(APawn* InPawn) override;

	// Controller开始游戏时调用，用于初始化UI，代替在Begin play中初始化UI
	// 确保玩家状态设置早于UI
	virtual void BeginPlayingState() override;

	UFUNCTION(BlueprintImplementableEvent)
	void BlueprintBeginPlayingState();

	void OnRep_PlayerState() override;

public:

	UFUNCTION(BlueprintCallable)
	void ToggleMissionSettlementWidget();  // 任务结算
};
