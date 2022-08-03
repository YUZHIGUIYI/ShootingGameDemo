// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "GameFramework/GameModeBase.h"
#include "SGGameModeBase.generated.h"

class UDataTable;
class USGAIDataAsset;
class UEnvQuery;
class UEnvQueryInstanceBlueprintWrapper;
class UCurveFloat;

// 数据表行 - 为在Game Mode中生成AI提供信息
USTRUCT(BlueprintType)
struct FAIInfoRow : public FTableRowBase
{
	GENERATED_BODY()

public:

	FAIInfoRow() : Weight(1.0f), Cost(5.0f), Reward(20.0f) {}

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FPrimaryAssetId AIId;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Weight;  // 考虑到有不同的AI，生成权重不一致

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Cost;  // 生成所需分数

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Reward;  // AI击杀奖励
};


UCLASS()
class SHOOTINGGAMEDEMO_API ASGGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly, Category = "AI Information")
	UDataTable* AIDataTable;  // AI数据表

	UPROPERTY(EditDefaultsOnly, Category = "AI Information")
	UEnvQuery* SpawnAIQuery;

	UPROPERTY(EditDefaultsOnly, Category = "AI Information")
	UCurveFloat* SpawnAICurve;  // 不同时间生成AI数量的描述曲线

	UPROPERTY(EditDefaultsOnly, Category = "AI Information")
	float CooldownBetweenFailure;  // 生成失败的间隔时间

	UPROPERTY(EditDefaultsOnly, Category = "AI Information")
	float SpawnTimeInterval;  // AI生成间隔

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI Information")
	int32 CreditsPerKill;

	FTimerHandle TimerHandle_SpawnAI;

protected:

	UFUNCTION()
	void SpawnAITimerElapsed();  // 生成AI Timer函数

	UFUNCTION()
	void OnAISpawnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);  //??
	
	void OnAIDataLoaded(FPrimaryAssetId LoadedId, FVector SpawnLocation);  // ??

	UFUNCTION()
	void RespawnPlayerElapsed(AController* Controller);  // Player重生

public:

	ASGGameModeBase();

	virtual void OnActorKilled(AActor* VictimActor, AActor* Killer);

	virtual void StartPlay() override;

	// SaveGame系统相关 - 重写InitGame
	void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

	// SaveGame系统相关 - 重写HandleStartingNewPlayer - 表示Player已准备好进入游戏，这可能会启动游戏
	void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;

	/*// SaveGame系统 - 载入游戏
	void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

	// SaveGame系统
	void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;

	// SaveGame系统
	UFUNCTION(BlueprintCallable, Category = "SaveGame")
	void WriteSaveGame();

	void LoadSaveGame();*/
};
