// Fill out your copyright notice in the Description page of Project Settings.


#include "GameSubsystems/SGGameMissionSubsystem.h"

bool USGGameMissionSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	return Super::ShouldCreateSubsystem(Outer);
}

void USGGameMissionSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void USGGameMissionSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

void USGGameMissionSubsystem::AddScore(int32 Delta)
{
	if ((Score + Delta) < 0)
	{
		UE_LOG(LogTemp, Log, TEXT("Can not lower than zero!"));
		return;
	}
	Score += Delta;
	OnMissionChanged.Broadcast(Score, NumOfKills);
}

void USGGameMissionSubsystem::AddNumOfKills(int32 Delta)
{
	if (Delta <= 0)
	{
		return;
	}
	NumOfKills += Delta;
	OnMissionChanged.Broadcast(Score, NumOfKills);
}
