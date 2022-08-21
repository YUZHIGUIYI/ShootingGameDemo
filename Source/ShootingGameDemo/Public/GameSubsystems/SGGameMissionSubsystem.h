// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SGGameMissionSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FMissionChanged, int32, NewScore, int32, NewNumOfKills);

UCLASS(meta = (DisplayName = "GameMission Subsystem"), Blueprintable)
class SHOOTINGGAMEDEMO_API USGGameMissionSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
protected:

	UPROPERTY(BlueprintReadOnly)
	int32 Score;

	UPROPERTY(BlueprintReadOnly)
	int32 NumOfKills;
	
public:
	
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	virtual void Deinitialize() override;

public:

	UFUNCTION(BlueprintCallable)
	void AddScore(int32 Delta);

	UFUNCTION(BlueprintCallable)
	void AddNumOfKills(int32 Delta);

	UPROPERTY(BlueprintAssignable)
	FMissionChanged OnMissionChanged;
};
