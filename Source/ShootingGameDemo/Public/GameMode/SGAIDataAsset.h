// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SGAIDataAsset.generated.h"

class USGAction;

UCLASS()
class SHOOTINGGAMEDEMO_API USGAIDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI Info")
	TSubclassOf<AActor> AIClass;

	// Actions or buffs to grant AI
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI Info")
	TArray<TSubclassOf<USGAction>> AIActions;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI Info")
	UTexture2D* AIIcon;

	FPrimaryAssetId GetPrimaryAssetId() const override;
};
