// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SGExplosiveEffect.h"
#include "Weapons/SGAmmo.h"
#include "SGRocketAmmo.generated.h"


UCLASS()
class SHOOTINGGAMEDEMO_API ASGRocketAmmo : public ASGAmmo
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage|Explode")
	float ExplodeDamage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage|Explode")
	float ExplodeRadius;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage|Explode")
	float ExplodeStrength;

	UPROPERTY(EditDefaultsOnly, Category = "Damage|ExplosiveEffect")
	TSubclassOf<ASGExplosiveEffect> ExplosiveEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage|Query")
	TArray<TEnumAsByte<EObjectTypeQuery>> HitObjectTypes;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage|Ignore")
	TArray<AActor*> ActorsToIgnore;

	UPROPERTY()
	TArray<AActor*> OutActors;
	
protected:
	
	virtual void OnAmmoStop(const FHitResult& ImpactResult) override;

	virtual void PostInitializeComponents() override;

	virtual void SpawnEmitterAtLocation(const FHitResult& ImpactResult) override;

	virtual void PlaySoundAtLocation(const FHitResult& ImpactResult) override;

	virtual void PlayCameraShake() override;

	void TransmitExplosiveDamage(AActor* ActorInRange, const FHitResult& ImpactResult);

public:

	ASGRocketAmmo();
	
};
