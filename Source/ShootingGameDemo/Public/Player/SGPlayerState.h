// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameMode/SGItemDataAsset.h"
#include "GameFramework/PlayerState.h"
#include "SGPlayerState.generated.h"

class ASGPlayerState;
class USGSaveGame;
class UDataTable;

// 积分动态多播委托
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnCreditsChanged, ASGPlayerState*, PlayerState, int32, NewCredits, int32, Delta);
// 个人记录时间动态多播委托
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnRecordTimeChanged, ASGPlayerState*, PlayerState, float, NewRecordTime, float, OldRecordTime);
// 背包改变委托
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnBackpackStateChanged, ASGPlayerState*, PlayerState, EItemTypeNew, ItemType);

UCLASS()
class SHOOTINGGAMEDEMO_API ASGPlayerState : public APlayerState
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Item Information")
	UDataTable* ItemDataTable;

	UPROPERTY(EditDefaultsOnly, Category = "Credits")
	int32 Credits;  // 积分

	UPROPERTY()
	int32 NumOfKills;  // 击杀AI数量

	UPROPERTY(BlueprintReadOnly)
	float PersonalRecordTime;

	UPROPERTY(BlueprintReadOnly, Category = "BackpackState")
	TMap<FName, int32> MedicalMap;
	
	UPROPERTY(BlueprintReadOnly, Category = "BackpackState")
	TMap<FName, int32> WeaponMap;

	UPROPERTY(BlueprintReadOnly, Category = "BackpackState")
	TMap<FName, FName> DataRowMap;

	UPROPERTY()
	TMap<FName, int32> PriorityMap;

protected:

	UFUNCTION(BlueprintCallable, Category = "BackpackEvent")
	bool PurchaseAndAddToBackpack(FName RowName, int32 ObjectNum, bool IsGoods = true);

	UFUNCTION(BlueprintCallable, Category = "BackpackEvent")
	bool DecreaseOrRemovalOfItem(FName ItemName, EItemTypeNew ItemType, int32 ObjectNum);

	UFUNCTION(BlueprintCallable, Category = "BackpackEvent")
	void SortBackpack();

	UFUNCTION(BlueprintCallable, Category = "BackpackEvent")
	void ApplyItemInBackpack(FName RowName, int32 ObjectNum = 1);

	UFUNCTION(BlueprintCallable, Category = "BackpackEvent")
	void ApplySpecialItemInBackpack(const FItemInfoRowNew& SelectedRow, int32 ObjectNum);

	void OnWeaponAssetLoaded(FItemInfoRowNew SelectedRow, FPrimaryAssetId LoadedId, int32 ObjectNum);
	
public:
	
	UPROPERTY(BlueprintAssignable, Category = "CreditEvent")
	FOnCreditsChanged OnCreditsChanged;

	UPROPERTY(BlueprintAssignable, Category = "RecordEvent")
	FOnRecordTimeChanged OnRecordTimeChanged;

	UPROPERTY(BlueprintAssignable, Category = "BackpackEvent")
	FOnBackpackStateChanged OnBackpackStateChanged;

public:

	UFUNCTION(BlueprintCallable)
	bool UpdatePersonalRecord(float NewTime);

	UFUNCTION(BlueprintCallable, Category = "Credits")
	int32 GetCredits() const;

	UFUNCTION(BlueprintCallable, Category = "Credits")
	void AddCredits(int32 Delta);

	UFUNCTION(BlueprintCallable, Category = "Credits")
	bool RemoveCredits(int32 Delta);

	UFUNCTION(BlueprintCallable, Category = "KillStatistics")
	int32 GetNumOfKills() const;
	
	UFUNCTION(BlueprintCallable, Category = "KillStatistics")
	void AddNumOfKills(int32 Delta);

	UFUNCTION(BlueprintNativeEvent)
	void SavePlayerState(USGSaveGame* SaveObject);

	UFUNCTION(BlueprintNativeEvent)
	void LoadPlayerState(USGSaveGame* SaveObject);

	UFUNCTION(BlueprintCallable, Category = "BackpaclEvent")
	bool PickupItemToBackpack(FName RowName);  // @Fixme
};
