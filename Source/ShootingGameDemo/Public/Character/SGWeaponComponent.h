// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SGWeaponComponent.generated.h"

class ASGWeaponBase;
class ASGCharacterBase;

// 弹夹状态改变
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnClipStatusChanged, USGWeaponComponent*, OwningComp, int32, PrimaryValue, int32, MaxValue);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHOOTINGGAMEDEMO_API USGWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WeaponSocket")
	TArray<FName> SocketArray{"EquipSocket", "MountSocket1", "MountSocket2"};

	UPROPERTY(BlueprintReadWrite, Category = "WeaponComponent")
	ASGWeaponBase* CurrentWeapon;  // 装备插槽上的武器指针

	UPROPERTY(BlueprintReadOnly, Category = "WeaponComponent")
	TMap<FName, ASGWeaponBase*> WeaponMap;  // 仅存储挂载武器信息

	UPROPERTY(BlueprintReadOnly, Category = "WeaponComponent")
	int32 WeaponNum;

	UPROPERTY(BlueprintAssignable, Category = "WeaponComponent")
	FOnClipStatusChanged OnClipStatusChanged;
	
public:

	UFUNCTION(BlueprintCallable, Category = "WeaponComponent")
	static USGWeaponComponent* GetWeaponComponent(AActor* FromActor);
	
	void StartFireAction(ASGCharacterBase* InstigatorCharacter);

	void StopFireAction(ASGCharacterBase* InstigatorCharacter);

	UFUNCTION()
	void OnDelayFinished();

	UFUNCTION()
	void StartFireDelay_Elapsed(ASGCharacterBase* InstigatorCharacter);

	UFUNCTION(BlueprintCallable, Category = "WeaponComponent")
	bool EquipWeapon(ASGCharacterBase* InstigatorCharacter);

	UFUNCTION(BlueprintCallable, Category = "WeaponComponent")
	bool MountWeapon(ASGCharacterBase* InstigatorCharacter);

	UFUNCTION(BlueprintCallable, Category = "WeaponComponent")
	void ReplaceClipCheck(ASGCharacterBase* InstigatorCharacter);

	UFUNCTION(BlueprintCallable, Category = "WeaponComponent")
	void ReplaceClipBegin(ASGCharacterBase* InstigatorCharacter);

	UFUNCTION(BlueprintCallable, Category = "WeaponComponent")
	void DiscardWeapon(ASGCharacterBase* InstigatorCharacter);

	UFUNCTION(BlueprintCallable, Category = "WeaponComponent")
	void DiscardAllWeapons(ASGCharacterBase* InstigatorCharacter);
	
	USGWeaponComponent();

protected:

	bool bCanFire;

	FTimerHandle TimerHandle_FireDelay;

public:	
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
};
