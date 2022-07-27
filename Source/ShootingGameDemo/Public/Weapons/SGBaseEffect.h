// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SGBaseEffect.generated.h"

class UParticleSystem;
class USoundCue;
class UMaterial;

UCLASS()
class SHOOTINGGAMEDEMO_API ASGBaseEffect : public AActor
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "BaseEffects")
	UParticleSystem* MetalVFX;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "BaseEffects")
	USoundCue* MetalSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "BaseEffects")
	UMaterial* DamageMaterial;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "BaseEffects")
	float ExistTime;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "BaseEffects")
	float DamageRegion;

	UPROPERTY(BlueprintReadWrite, Category = "HitResult")
	FHitResult OnHitResult;
	
public:	
	
	ASGBaseEffect();

protected:
	
	virtual void BeginPlay() override;

public:	
	
	virtual void Tick(float DeltaTime) override;

};
