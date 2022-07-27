// Fill out your copyright notice in the Description page of Project Settings.


#include "ShootingGameDemo/Public/Player/SGPlayerState.h"
#include "Player/SGSaveGame.h"

void ASGPlayerState::AddCredits(int32 Delta)
{
	if (ensure(Delta >= 0.0f))
	{
		Credits += Delta;
		OnCreditsChanged.Broadcast(this, Credits, Delta);
	}
}

bool ASGPlayerState::RemoveCredits(int32 Delta)
{
	if (!ensure(Delta >= 0.0f))
	{
		return false;
	}
	if (Credits < Delta)
	{
		return false;
	}

	Credits -= Delta;

	OnCreditsChanged.Broadcast(this, Credits, -Delta);

	return true;
}

void ASGPlayerState::SavePlayerState_Implementation(USGSaveGame* SaveObject)
{
	if (SaveObject)
	{
		// 将PlayerState中的数据存入SGSaveGame中的SavedPlayers中
		FPlayerSaveData SavedData;
		SavedData.Credits = Credits;
		SavedData.PersonalRecordTime = PersonalRecordTime;
		SavedData.PlayerID = GetUniqueId().ToString();  // 获取唯一Id，转换为FString

		// PlayerCharacter是否存活
		if (APawn* PlayerPawn = GetPawn())
		{
			SavedData.Location = PlayerPawn->GetActorLocation();
			SavedData.Rotation = PlayerPawn->GetActorRotation();
			//  ??? 待实现
		}

		SaveObject->SavedPlayers.Add(SavedData);
	}
}

void ASGPlayerState::LoadPlayerState_Implementation(USGSaveGame* SaveObject)
{
	if (SaveObject)
	{
		FPlayerSaveData* FoundPlayerSaveData = SaveObject->GetPlayerData(this);
		if (FoundPlayerSaveData)
		{
			AddCredits(FoundPlayerSaveData->Credits);
			PersonalRecordTime = FoundPlayerSaveData->PersonalRecordTime;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Could not find SaveGame Data for player id: '%i'."), GetPlayerId());
		}
	}
}

bool ASGPlayerState::UpdatePersonalRecord(float NewTime)
{
	if (NewTime > PersonalRecordTime)
	{
		float OldRecordTime = PersonalRecordTime;
		PersonalRecordTime = NewTime;
		OnRecordTimeChanged.Broadcast(this, PersonalRecordTime, OldRecordTime);
		return true;
	}

	return false;
}

int32 ASGPlayerState::GetCredits() const
{
	return Credits;
}


