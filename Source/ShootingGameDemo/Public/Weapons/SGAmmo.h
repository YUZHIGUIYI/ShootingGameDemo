// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SGBaseEffect.h"
#include "GameFramework/Actor.h"
#include "Sound/SoundCue.h"
#include "SGAmmo.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UParticleSystemComponent;
class UCameraShakeBase;

USTRUCT(BlueprintType)
struct FEffectInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "BaseEffects")
	UParticleSystem* MetalVFX;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "BaseEffects")
	USoundCue* MetalSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "BaseEffects")
	UParticleSystem* BodyVFX;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "BaseEffects")
	USoundCue* BodySound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "BaseEffects")
	UMaterial* DamageMaterial;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "BaseEffects")
	UMaterialInstance* DamageDecal;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "BaseEffects")
	float ExistTime;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "BaseEffects")
	float DamageRegion;
};

UENUM(BlueprintType)
enum class EAmmoType : uint8
{
	Rifle_ammo,
	Sniper_ammo,
	Shotgun_ammo,
	Rocket_ammo,
	Grenade_ammo
};

UCLASS()
class SHOOTINGGAMEDEMO_API ASGAmmo : public AActor
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AmmoType")
	EAmmoType AmmoType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EffectType")
	FEffectInfo EffectInfo;
	//TSubclassOf<ASGBaseEffect> EffectType;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Speed")
	float InitialSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Speed")
	float MaxSpeed;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	float DirectDamage;

	UPROPERTY(EditDefaultsOnly, Category = "Shake")
	TSubclassOf<UCameraShakeBase> ImpactShake;

	UPROPERTY(EditDefaultsOnly, Category = "Shake")
	float ImpactShakeInnerRadius;

	UPROPERTY(EditDefaultsOnly, Category = "Shake")
	float ImpactShakeOuterRadius;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USphereComponent* SphereComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* StaticMeshComp;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UProjectileMovementComponent* MoveComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UParticleSystemComponent* ParticleSystemComp;

protected:

	FTransform SpawnTM;

protected:

	UFUNCTION()
	virtual void OnAmmoStop(const FHitResult& ImpactResult);

	virtual void SpawnEmitterAtLocation(const FHitResult& ImpactResult);

	virtual void PlaySoundAtLocation(const FHitResult& ImpactResult);

	virtual void PlayCameraShake();  // 弃用

	void SpawnDecalAttached(const FHitResult& ImpactResult);

	virtual void PostInitializeComponents() override;

public:
	
	ASGAmmo();

protected:
	
	virtual void BeginPlay() override;

public:	
	
	virtual void Tick(float DeltaTime) override;

};
