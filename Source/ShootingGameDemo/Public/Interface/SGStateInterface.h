// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SGStateInterface.generated.h"

UINTERFACE(MinimalAPI)
class USGStateInterface : public UInterface
{
	GENERATED_BODY()
};

class SHOOTINGGAMEDEMO_API ISGStateInterface
{
	GENERATED_BODY()

public:
	
	// 暂无涌出
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	float GetForwardVelocity();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	float GetRightVelocity();
};
