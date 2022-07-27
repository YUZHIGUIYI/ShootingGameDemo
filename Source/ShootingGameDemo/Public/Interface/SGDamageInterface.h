// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SGDamageInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USGDamageInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SHOOTINGGAMEDEMO_API ISGDamageInterface
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void TakeDirectDamage(APawn* InstigatorPawn, const FHitResult& ImpactResult, float Damage);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void TakeExplosiveDamage(APawn* InstigatorPawn, const FHitResult& ImpactResult, float Damage);
	
};
