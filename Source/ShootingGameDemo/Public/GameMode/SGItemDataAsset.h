// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SGItemDataAsset.generated.h"


UCLASS()
class SHOOTINGGAMEDEMO_API USGItemDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Info")
	TSubclassOf<AActor> ItemClass;

	FPrimaryAssetId GetPrimaryAssetId() const override;
};
