// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SGSaveGame.generated.h"

USTRUCT()
struct FActorSaveData
{
	GENERATED_BODY()

public:

	UPROPERTY()
	FString ActorName;

	UPROPERTY()
	FName NewActorName;  // Actor标识符

	UPROPERTY()
	FTransform Transform;  // Actor的Location、Rotation与Scale

	UPROPERTY()
	TArray<uint8> ByteData;  // 可以包含Actor的所有SaveGame UPROPERTY() 变量
};

USTRUCT()
struct FPlayerSaveData
{
	GENERATED_BODY()

public:

	UPROPERTY()
	FString PlayerID;  // 在线游戏Player识别标识

	UPROPERTY()
	int32 Credits;  // Player积分

	UPROPERTY()
	float PersonalRecordTime;

	UPROPERTY()
	FVector Location;

	UPROPERTY()
	FRotator Rotation;

	// ???
};

UCLASS()
class SHOOTINGGAMEDEMO_API USGSaveGame : public USaveGame
{
	GENERATED_BODY()

public:

	UPROPERTY()
	int32 Credits;

	UPROPERTY()
	TArray<FPlayerSaveData> SavedPlayers;

	UPROPERTY()
	TArray<FActorSaveData> SavedActors;

public:

	FPlayerSaveData* GetPlayerData(APlayerState* PlayerState);
};
