// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Engine/DataTable.h"
#include "SGItemDataAsset.generated.h"

UENUM(BlueprintType)
enum class EItemTypeNew : uint8
{
	MedicalSupply,
	AmmoSupply,
	Weapon
};

USTRUCT(BlueprintType)
struct FItemInfoRowNew : public FTableRowBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EItemTypeNew ItemTypeNew;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FPrimaryAssetId ItemId;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTexture2D* Icon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 ItemEffect;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 Credits;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 Priority;
};

UCLASS()
class SHOOTINGGAMEDEMO_API USGItemDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Info")
	TSubclassOf<AActor> ItemClass;

	FPrimaryAssetId GetPrimaryAssetId() const override;
};
