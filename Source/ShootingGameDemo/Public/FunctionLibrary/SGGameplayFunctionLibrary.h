// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SGGameplayFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTINGGAMEDEMO_API USGGameplayFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "GamePlayFunctionLibrary")
	static bool ApplyNormalDamage(AActor* DamageCauser, AActor* TargetActor, const FHitResult& ImpactResult, float DirectDamage);

	UFUNCTION(BlueprintCallable, Category = "GamePlayFunctionLibrary")
	static bool CheckGroupByGameplayTag(AActor* DamageCauser, AActor* TargetActor);
};
