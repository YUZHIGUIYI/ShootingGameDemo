// Fill out your copyright notice in the Description page of Project Settings.


#include "GameSubsystems/SGGameMissionSubsystem.h"

void USGGameMissionSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void USGGameMissionSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

void USGGameMissionSubsystem::HandleMissionBegin(AController* NewPlayer)
{
	CreateMissionTipInterface(NewPlayer);
}

void USGGameMissionSubsystem::HandleMissionEnd(AController* NewPlayer)
{
	CreateMissionSettlementInterface(NewPlayer);
}
