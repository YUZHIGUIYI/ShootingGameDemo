// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/SGSaveGame.h"
#include "GameFramework/PlayerState.h"

FPlayerSaveData* USGSaveGame::GetPlayerData(APlayerState* PlayerState)
{
	if (PlayerState == nullptr)
	{
		return nullptr;
	}

	// PIE模式不会提供唯一的ID，所以在PIE测试时跳过
	// UObjects无法访问UWorld，因此需要通过PlayerState获取
	if (PlayerState->GetWorld()->IsPlayInEditor())
	{
		UE_LOG(LogTemp, Log, TEXT("In PIE Mode, we cannot use PlayerID to retrieve Saved Player data. Using first entry in array if available."));
		if (SavedPlayers.IsValidIndex(0))
		{
			return &SavedPlayers[0];
		}
		return nullptr;
	}

	// 非PIE模式 - 返回PlayerState唯一的online id - 见UObject
	FString PlayerID = PlayerState->GetUniqueId().ToString();
	// lambda表达式 - 找到符合PlayerID的FPlayerSaveData
	return SavedPlayers.FindByPredicate([&] (const FPlayerSaveData& Data)
	{
		return Data.PlayerID == PlayerID;
	});
}
