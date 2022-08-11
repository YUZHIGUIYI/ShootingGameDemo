// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SGGameplayFunctionLibrary.generated.h"


UCLASS()
class SHOOTINGGAMEDEMO_API USGGameplayFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "GamePlayFunctionLibrary")
	static bool ApplyNormalDamage(AActor* DamageCauser, AActor* TargetActor, const FHitResult& ImpactResult, float DirectDamage);

	UFUNCTION(BlueprintCallable, Category = "GamePlayFunctionLibrary")
	static void ShiftOfPerspectives(AActor* PlayerActor, AActor* InteractiveActor, bool bIsVisible);

	UFUNCTION(BlueprintCallable, Category = "GamePlayFunctionLibrary")
	static bool ApplyActionEffect(AActor* InstigatorActor, AActor* TargetActor, float EffectValue);

	UFUNCTION(BlueprintCallable, Category = "GamePlayFunctionLibrary")
	static bool RequireStrength(AActor* InstigatorActor, float DecreaseSpeed);

	UFUNCTION(BlueprintCallable, Category = "GamePlayFunctionLibrary")
	static bool CheckGroupByGameplayTag(AActor* DamageCauser, AActor* TargetActor);

	UFUNCTION(BlueprintCallable, Category = "GamePlayFunctionLibrary")
	static bool CheckCreditsWhenPurchasing(AActor* InstigatorActor, int32 Cost);

	UFUNCTION(BlueprintCallable, Category = "GamePlayFunctionLibrary")
	static bool ApplyHealing(AActor* InstigatorActor, float HealingAmount);

	UFUNCTION(BlueprintCallable, Category = "GamePlayFunctionLibrary")
	static bool ApplyClip(AActor* InstigatorActor, FName ClipName, int32 AmmoAmount);
	
};
