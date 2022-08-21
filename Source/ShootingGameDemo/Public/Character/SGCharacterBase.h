// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SGAttributeSet.h"
#include "Abilities/GameplayAbility.h"
#include "GameFramework/Character.h"
#include "Interface/SGDamageInterface.h"
#include "SGCharacterBase.generated.h"

class USpringArmComponent;
class UCameraComponent;
class USGInteractionComponent;
class USGAttributeComponent;
class USGWeaponComponent;
class ASGWeaponBase;
class USGActionComponent;
class USGBackpackComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnShowEquipmentStatus, bool, bEquipWeapon, ASGWeaponBase*, Weapon);

UCLASS()
class SHOOTINGGAMEDEMO_API ASGCharacterBase : public ACharacter, public ISGDamageInterface
{
	GENERATED_BODY()

protected:
	
	UPROPERTY(BlueprintReadWrite, Category = "WeaponEvent")
	bool bIsAiming;
	
	// 实现后坐力
	bool bIsRecoiling;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Recoil")
	float RecoilRate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Recoil")
	float RecoilPitchUp;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Recoil")
	float RecoilPitchDown;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Recoil")
	float RecoilYawLeft;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Recoil")
	float RecoilYawRight;

	float FinalRecoilPitch;

	float FinalRecoilYaw;

	float FinalRecoilYawDirection;  // ???

	FTimerHandle TimerHandle_Recoil;

public:

	UPROPERTY(EditDefaultsOnly, Category = "AnimMontage")
	UAnimMontage* EquipOrMountMontage;

	UPROPERTY(EditDefaultsOnly, Category = "AnimMontage")
	UAnimMontage* FireMontageNormal;  // 弃用，转移至SGAction_Fire

	UPROPERTY(EditDefaultsOnly, Category = "AnimMontage")
	UAnimMontage* FireMontageShotgun;  // 弃用，转移至SGAction_Fire

	UPROPERTY(EditDefaultsOnly, Category = "AnimMontage")
	UAnimMontage* ReplaceClipMontage;

	// 声明Ability数组
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterAbilities")
	TArray<TSubclassOf<UGameplayAbility>> CharacterAbilities;

	UPROPERTY(BlueprintReadWrite, Category = "CharacterAttributes")
	bool bEquipWeapon;

	UPROPERTY(BlueprintAssignable, Category = "CharacterAttributes")
	FOnShowEquipmentStatus OnShowEquipmentStatus;

	UPROPERTY(BlueprintAssignable, Category = "CharacterAttributes")
	FOnShowEquipmentStatus OnShowHitMarkStatus;
	
public:
	
	// Sets default values for this character's properties
	ASGCharacterBase();

	UFUNCTION(BlueprintCallable, Category = "BaseCharacter")
	void ChangeActionMode();

	void BeginRecoil();

	UFUNCTION()
	void StopRecoil();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USGInteractionComponent* InteractionComp;  // 交互组件

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USGAttributeComponent* AttributeComp;  // 属性组件

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USGWeaponComponent* WeaponComp;  // 武器组件

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USGActionComponent* ActionComp;  // 动作组件

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USGBackpackComponent* BackpackComp;  // 背包组件

protected:
	
	void MoveForward(float value);

	void MoveRight(float value);

	void SprintStart();  // 开始加速奔跑
	void SprintStop();   // 停止加速奔跑

	void PrimaryInteract();

	UFUNCTION(BlueprintCallable, Category = "WeaponEvent")
	void PrimaryFire();

	UFUNCTION(BlueprintCallable, Category = "WeaponEvent")
	void StopFire();

	UFUNCTION(BlueprintNativeEvent, Category = "WeaponEvent")
	void AimingActionStart();
	
	UFUNCTION(BlueprintNativeEvent, Category = "WeaponEvent")
	void AimingActionStop();

	UFUNCTION(BlueprintNativeEvent, Category = "WeaponEvent")
	void SwitchWeaponAction();

	UFUNCTION(BlueprintCallable, Category = "WeaponEvent")
	void EquipOrMountAction();

	UFUNCTION(BlueprintCallable, Category = "WeaponEvent")
	void SwitchWeapon();

	UFUNCTION(BlueprintCallable, Category = "WeaponEvent")
	void ReplaceClipAction();

	UFUNCTION(BlueprintCallable, Category = "WeaponEvent")
	void DiscardAction();
	
	virtual void BeginPlay() override;

protected:

	// 对伤害或治疗做出反应
	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, USGAttributeComponent* OwningComp, float NewHealth, float Delta);

	// 用于绑定 OnHealthChanged 函数
	virtual void PostInitializeComponents() override;

public:

	UFUNCTION(BlueprintImplementableEvent, Category = "WeaponEvent")
	void RecoilForce();  // 在蓝图中实现，暂不使用

	// 修正交互位置 - 从摄像机角度出发
	virtual FVector GetPawnViewLocation() const override;  // @fix change from protected to public?

	FVector GetCameraForwardVector() const;

	void SetSpringArmLength(float LengthValue = 300.0f);

	void SetFieldOfView(float View = 90.0f);
	
	virtual void Tick(float DeltaTime) override;
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void TakeDirectDamage_Implementation(APawn* InstigatorPawn, const FHitResult& ImpactResult, float Damage) override;

	void StopActionsFeedBack();
};
