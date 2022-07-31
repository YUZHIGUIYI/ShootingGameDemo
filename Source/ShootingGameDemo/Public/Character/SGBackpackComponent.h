// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"
#include "SGBackpackComponent.generated.h"

class USGItemDataAsset;
class UDataTable;

// 枚举 - Item的类型
UENUM(BlueprintType)
enum class EItemType : uint8
{
	MedicalKit,
	Weapon
};

// 数据表行 - 提供Item的具体信息
USTRUCT(BlueprintType)
struct FItemInfoRow : public FTableRowBase
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EItemType ItemType;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FPrimaryAssetId ItemId;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 Credits;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 ItemEffect;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTexture2D* Icon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 Priority;
};

USTRUCT(BlueprintType)
struct FItemArray
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite)
	TArray<int32> IArray{0, 0};  // Number and Priority
};

// 动态多播委托
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnBackpackChanged, USGBackpackComponent*, OwningComp, EItemType, ItemType);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHOOTINGGAMEDEMO_API USGBackpackComponent : public UActorComponent
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Item Information")
	UDataTable* ItemDataTable;

public:

	UPROPERTY(BlueprintReadOnly, Category = "BackpackComponent")
	TMap<FName, FName> NameToRowMap;

	UPROPERTY(BlueprintReadWrite, Category = "BackpackComponent")
	TMap<FName, FItemArray> BackpackMedicalMap;

	UPROPERTY(BlueprintReadWrite, Category = "BackpackComponent")
	TMap<FName, FItemArray> BackpackWeaponMap;
	
public:	
	
	USGBackpackComponent();

	UPROPERTY(BlueprintAssignable)
	FOnBackpackChanged OnBackpackChanged;

protected:

	UFUNCTION(BlueprintCallable, Category = "BackpackComponent")
	void CheckCreditsAndPurchase(FName ObjectName, FName RowName, EItemType ItemType, int32 ObjectNum, int32 Priority);  // @Fixme: too many arguments
	
	UFUNCTION(BlueprintCallable, Category = "BackpackComponent")
	void AddIntoBackpack(FName ObjectName, FName RowName, EItemType ItemType, int32 ObjectNum, int32 Priority);  // @Fixme: too many arguments

	UFUNCTION(BlueprintCallable, Category = "BackpackComponent")
	void SortBackpack();

	UFUNCTION(BlueprintCallable, Category = "BackpackComponent")
	bool RemoveFromBackpack(FName ObjectName);

	void DecreaseOfItemNumber(FName ObjectName, EItemType ItemType);

	UFUNCTION(BlueprintCallable, Category = "BackpackComponent")
	void ApplyGeneralItem(FName ObjectName, FName RowName, EItemType ItemType); // @Fixme: too many arguments

	UFUNCTION(BlueprintCallable, Category = "BackpackComponent")
	void ApplySpecialItem(const FItemInfoRow& SelectedRow, FName ObjectName, EItemType ItemType);  // @Fixme: too many arguments

	void OnSpecialItemLoaded(FPrimaryAssetId LoadedId, FName ObjectName, EItemType ItemType);  // @Fixme: too many arguments
};
