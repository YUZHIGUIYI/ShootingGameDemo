// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SGSaveGameSubsystem.generated.h"

class USGSaveGame;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSaveGameSignal, class USGSaveGame*, SaveGameObject);

// 该子系统用于处理游戏状态和玩家数据的保存与加载
UCLASS(meta = (DisplayName = "SaveGame Subsystem"))  // 蓝图节点中的名称
class SHOOTINGGAMEDEMO_API USGSaveGameSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

protected:

	FString SlotName;  // 保存与加载SaveGame的插槽名称

	UPROPERTY()
	USGSaveGame* CurrentSaveGame;

public:

	UPROPERTY(BlueprintAssignable)
	FOnSaveGameSignal OnSaveGameLoaded;

	UPROPERTY(BlueprintAssignable)
	FOnSaveGameSignal OnSaveGameWritten;
	
public:

	// 将序列化数据从PlayState导入到Player中 - 此函数在USGGameModeBase中调用
	void HandleRestorePlayerData(AController* NewPlayer);

	// 重写 - 初始化Subsystem
	void Initialize(FSubsystemCollectionBase& Collection) override;

	// 在初始化完成后，更新Player的出生点位置数据
	UFUNCTION(BlueprintCallable)
	bool OverrideSpawnTransform(AController* NewPlayer);

	// 更改插槽名称
	UFUNCTION(BlueprintCallable)
	void ChangeSlotName(FString NewSlotName);

	// 保存游戏
	UFUNCTION(BlueprintCallable, Category = "SaveGame")
	void WriteSaveGame();

	// 加载游戏
	void LoadSaveGame(FString LoadedSlotName = "");
	
};
