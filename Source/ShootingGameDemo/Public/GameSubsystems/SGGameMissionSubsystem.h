// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SGGameMissionSubsystem.generated.h"

/**
 * 
 */
UCLASS(meta = (DisplayName = "GameMission Subsystem"), Blueprintable)
class SHOOTINGGAMEDEMO_API USGGameMissionSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:

	void Initialize(FSubsystemCollectionBase& Collection) override;

	void Deinitialize() override;

	void HandleMissionBegin(AController* NewPlayer);

	UFUNCTION(BlueprintImplementableEvent, Category = "GameMission")
	void CreateMissionTipInterface(AController* NewPlayer);

	void HandleMissionEnd(AController* NewPlayer);

	UFUNCTION(BlueprintImplementableEvent, Category = "GameMission")
	void CreateMissionSettlementInterface(AController* NewPlayer);
};
