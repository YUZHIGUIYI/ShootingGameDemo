// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "SGSaveGameSettings.generated.h"

UCLASS(Config=Game, DefaultConfig, meta = (DisplayName = "Save Game Settings"))  // SaveGame仅用于默认配置
class SHOOTINGGAMEDEMO_API USGSaveGameSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:

	// 默认的SaveGame插槽名称
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "GeneralSettings")
	FString SaveGameSlotName;

public:

	USGSaveGameSettings();
};
