// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "SGPlayerState.generated.h"

class ASGPlayerState;
class USGSaveGame;

// 积分动态多播委托
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnCreditsChanged, ASGPlayerState*, PlayerState, int32, NewCredits, int32, Delta);
// 个人记录时间动态多播委托
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnRecordTimeChanged, ASGPlayerState*, PlayerState, float, NewRecordTime, float, OldRecordTime);

UCLASS()
class SHOOTINGGAMEDEMO_API ASGPlayerState : public APlayerState
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Credits")
	int32 Credits;

	UPROPERTY(BlueprintReadOnly)
	float PersonalRecordTime;
	
public:
	
	UPROPERTY(BlueprintAssignable, Category = "CreditEvent")
	FOnCreditsChanged OnCreditsChanged;

	UPROPERTY(BlueprintAssignable, Category = "RecordEvent")
	FOnRecordTimeChanged OnRecordTimeChanged;

public:

	UFUNCTION(BlueprintCallable)
	bool UpdatePersonalRecord(float NewTime);

	UFUNCTION(BlueprintCallable, Category = "Credits")
	int32 GetCredits() const;

	UFUNCTION(BlueprintCallable, Category = "Credits")
	void AddCredits(int32 Delta);

	UFUNCTION(BlueprintCallable, Category = "Credits")
	bool RemoveCredits(int32 Delta);

	UFUNCTION(BlueprintNativeEvent)
	void SavePlayerState(USGSaveGame* SaveObject);

	UFUNCTION(BlueprintNativeEvent)
	void LoadPlayerState(USGSaveGame* SaveObject);
};
