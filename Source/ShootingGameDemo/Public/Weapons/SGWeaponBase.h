// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SGAmmo.h"
#include "SGEmptyAmmo.h"
#include "GameFramework/Actor.h"
#include "Interface/SGGameplayInterface.h"
#include "SGWeaponBase.generated.h"

class ASGCharacterBase;

UENUM(BlueprintType)
enum class EWepaonType : uint8
{
	Rifle,
	Sniper_rifle,
	Shotgun,
	Rocket_launcher,
	Grenade_gun
};

UENUM(BlueprintType)
enum class EShootingType : uint8
{
	Single,
	Running
};

UENUM(BlueprintType)
enum class EBallisticType : uint8
{
	Projectile,
	Scattering
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FWeaponAmmoChangedDelegate, int32, OldValue, int32, NewValue);

class USkeletalMeshComponent;

UCLASS()
class SHOOTINGGAMEDEMO_API ASGWeaponBase : public AActor, public ISGGameplayInterface
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Socket")
	FName ChamberSocketName;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Socket")
	FName MuzzleSocketName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Socket")
	FName AttachedSocketName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|WeaponType")
	EWepaonType WeaponType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|WeaponType")
	FText WeaponName;  // 显示在UI上

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Ammo")
	TSubclassOf<ASGEmptyAmmo> EmptyAmmoType;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Ammo")
	TSubclassOf<ASGAmmo> AmmoType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon|Ammo")
	int32 ClipLoad;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon|Ammo")
	int32 PrimaryClipAmmo;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon|Ammo")
	int32 MaxPrimaryClipAmmo;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|FireSetting")
	float ShootingDistance;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|FireSetting")
	float ShootingSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|FireSetting")
	float RecoilOffset;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|FireSetting")
	EShootingType ShootingType;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|FireSetting")
	EBallisticType BallisticType;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Animation")
	UAnimSequence* FireSequence;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Animation")
	UAnimSequence* ReloadSequence;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Audio")
	USoundCue* PickupSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Audio")
	USoundCue* EmptyClipSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Audio")
	USoundCue* EquipSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|UI")
	UTexture2D* WepaonUI;

protected:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USkeletalMeshComponent* SkeletalMeshComp;

public:

	UPROPERTY(BlueprintAssignable, Category = "Weapon|AmmoInfo")
	FWeaponAmmoChangedDelegate OnPrimaryClipAmmoChanged;

	UPROPERTY(BlueprintAssignable, Category = "Weapon|AmmoInfo")
	FWeaponAmmoChangedDelegate OnMaxPrimaryClipAmmoChanged;

	UFUNCTION(BlueprintCallable, Category = "Weapon|SkeletalMesh")
	USkeletalMeshComponent* GetSkeletalMesh() const;
	
public:

	UFUNCTION(BlueprintCallable, Category = "Weapon|AmmoInfo")
	virtual int32 GetPrimaryClipAmmo() const;

	UFUNCTION(BlueprintCallable, Category = "Weapon|AmmoInfo")
	virtual int32 GetMaxPrimaryClipAmmo() const;

	UFUNCTION(BlueprintCallable, Category = "Weapon|AmmoInfo")
	virtual void SetPrimaryClipAmmo(int32 NewPrimaryClipAmmo);

	UFUNCTION(BlueprintCallable, Category = "Weapon|AmmoInfo")
	virtual void SetMaxPrimaryClipAmmo(int32 NewMaxPrimaryClipAmmo);

	UFUNCTION(BlueprintCallable, Category = "Weapon|Animation")
	UAnimSequence* GetReloadSequence() const;

	UFUNCTION(BlueprintCallable, Category = "Weapon|Sound")
	USoundCue* GetPickupSound() const;
	
	// 实现交互函数
	void Interact_Implementation(APawn* InstigatorPawn) override;

	FText GetInteractText_Implementation(APawn* InstigatorPawn) override;

	// 装备或挂载武器
	bool EquipOrMountWeapon(ASGCharacterBase* Character);
	
	ASGWeaponBase();

protected:
	
	virtual void BeginPlay() override;

public:	
	
	virtual void Tick(float DeltaTime) override;

};
