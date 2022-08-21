// Fill out your copyright notice in the Description page of Project Settings.


#include "ShootingGameDemo/Public/Character/SGCharacterBase.h"
#include "Camera/CameraComponent.h"
#include "Character/SGActionComponent.h"
#include "Character/SGAttributeComponent.h"
#include "Character/SGBackpackComponent.h"
#include "Character/SGInteractionComponent.h"
#include "Character/SGWeaponComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Player/SGPlayerController.h"

// Sets default values
ASGCharacterBase::ASGCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;

	// 组件实例化
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SprintArmComp");
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SetupAttachment(RootComponent);

	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp->SetupAttachment(SpringArmComp);

	bEquipWeapon = false;
	// Controller 旋转时不发生旋转
	bUseControllerRotationYaw = bEquipWeapon;
	// Character 移动
	GetCharacterMovement()->bOrientRotationToMovement = !bEquipWeapon;

	// 动作组件初始化
	ActionComp = CreateDefaultSubobject<USGActionComponent>("ActionComp");
	// 背包组件
	BackpackComp = CreateDefaultSubobject<USGBackpackComponent>("BackpackComp");
	// 武器组件实例化
	WeaponComp = CreateDefaultSubobject<USGWeaponComponent>("WeaponComp");
	// 交互组件实例化
	InteractionComp = CreateDefaultSubobject<USGInteractionComponent>("InteractionComp");
	// 属性组件实例化
	AttributeComp = CreateDefaultSubobject<USGAttributeComponent>("AttributeComp");

	bIsRecoiling = false;
	RecoilRate = 0.1f;
	RecoilPitchUp = 0.25f;
	RecoilPitchDown = -1.25f;
	RecoilYawLeft = -1.0f;
	RecoilYawRight = 1.0f;
}

void ASGCharacterBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	// 将 OnHealthChanged 函数绑定到 AttributeComp 组件
	AttributeComp->OnHealthChanged.AddDynamic(this, &ASGCharacterBase::OnHealthChanged);
}

void ASGCharacterBase::ChangeActionMode()
{
	bUseControllerRotationYaw = bEquipWeapon;
	GetCharacterMovement()->bOrientRotationToMovement = !bEquipWeapon;
}

void ASGCharacterBase::BeginPlay()
{
	Super::BeginPlay();
}

void ASGCharacterBase::OnHealthChanged(AActor* InstigatorActor, USGAttributeComponent* OwningComp, float NewHealth,
	float Delta)
{
	if (Delta < 0.0f)
	{
		// 考虑加入受击特效Effect
	}
	// Character 死亡
	if (NewHealth <= 0.0f && Delta < 0.0f)
	{
		//StopFire();  // 停止开火 - 防止出现连发枪支继续开火的Bug
		//AimingActionStop_Implementation();  // 停止瞄准
		StopActionsFeedBack();
		
		WeaponComp->DiscardAllWeapons(this);  // Character死亡时掉落所有武器
		bEquipWeapon = false;
		OnShowEquipmentStatus.Broadcast(bEquipWeapon, nullptr);
		
		APlayerController* PC = Cast<ASGPlayerController>(GetController());
		DisableInput(PC);  // 禁止输入事件
		SetLifeSpan(4.5f);  // Character销毁
	}
}

FVector ASGCharacterBase::GetPawnViewLocation() const
{
	return CameraComp->GetComponentLocation();  // 发生交互时从摄像机角度出发
}

FVector ASGCharacterBase::GetCameraForwardVector() const
{
	return CameraComp->GetForwardVector();
}

void ASGCharacterBase::SetSpringArmLength(float LengthValue)
{
	SpringArmComp->TargetArmLength = LengthValue;
}

void ASGCharacterBase::SetFieldOfView(float View)
{
	CameraComp->FieldOfView = View;
}

void ASGCharacterBase::Tick(float DeltaTime)
{
	// 后坐力
	if (bIsRecoiling)
	{
		AddControllerPitchInput(FinalRecoilPitch);
		AddControllerYawInput(FinalRecoilYaw);
	}
	
	Super::Tick(DeltaTime);
}

void ASGCharacterBase::BeginRecoil()
{
	FinalRecoilPitch = RecoilRate * FMath::FRandRange(RecoilPitchDown, RecoilPitchUp);
	FinalRecoilYaw = RecoilRate * FMath::FRandRange(RecoilYawLeft, RecoilYawRight);
	//FTimerDelegate Delegate;
	//Delegate.BindUFunction(this, "StopRecoil");
	//GetWorldTimerManager().SetTimer(TimerHandle_Recoil, Delegate, 0.05f, true);
	GetWorldTimerManager().SetTimer(TimerHandle_Recoil, this, &ASGCharacterBase::StopRecoil, 0.07f, true);  /// ???
	bIsRecoiling = true;
}

void ASGCharacterBase::StopRecoil()
{
	bIsRecoiling = false;
	GetWorldTimerManager().ClearTimer(TimerHandle_Recoil);  // ???
}

void ASGCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ASGCharacterBase::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASGCharacterBase::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("PrimaryInteract", IE_Pressed, this, &ASGCharacterBase::PrimaryInteract);
	
	PlayerInputComponent->BindAction("PrimaryFire", IE_Pressed, this, &ASGCharacterBase::PrimaryFire);
	PlayerInputComponent->BindAction("PrimaryFire", IE_Released, this, &ASGCharacterBase::StopFire);
	
	PlayerInputComponent->BindAction("EMWeapon", IE_Pressed, this, &ASGCharacterBase::EquipOrMountAction);
	PlayerInputComponent->BindAction("SwitchWeapon", IE_Pressed, this, &ASGCharacterBase::SwitchWeaponAction);
	PlayerInputComponent->BindAction("ReplaceClip", IE_Pressed, this, &ASGCharacterBase::ReplaceClipAction);
	
	PlayerInputComponent->BindAction("Aiming", IE_Pressed, this, &ASGCharacterBase::AimingActionStart);
	PlayerInputComponent->BindAction("Aiming", IE_Released, this, &ASGCharacterBase::AimingActionStop);

	PlayerInputComponent->BindAction("Discard", IE_Pressed, this, &ASGCharacterBase::DiscardAction);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ASGCharacterBase::SprintStart);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ASGCharacterBase::SprintStop);
}

void ASGCharacterBase::MoveForward(float value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;
	
	AddMovementInput(ControlRot.Vector(), value);
}

void ASGCharacterBase::MoveRight(float value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;

	FVector RightVector = FRotationMatrix(ControlRot).GetScaledAxis(EAxis::Y);
	AddMovementInput(RightVector, value);
}

void ASGCharacterBase::SprintStart()
{
	// 移动过程中进行短距离冲刺消耗体力值
	if (AttributeComp->GetStrength() > 1.0f && GetVelocity().Size() > 0.0f)
	{
		AttributeComp->bIsSprinting = true;
		ActionComp->StartActionByName(this, "Sprint");
	}
}

void ASGCharacterBase::SprintStop()
{
	AttributeComp->bIsSprinting = false;
	ActionComp->StopActionByName(this, "Sprint");
}

void ASGCharacterBase::PrimaryInteract()
{
	// 与场景中的可交互的Actor交互
	if (InteractionComp)
	{
		InteractionComp->PrimaryInteract();
	}
}

void ASGCharacterBase::PrimaryFire()
{
	if (WeaponComp->CurrentWeapon != nullptr && bEquipWeapon)
	{
		ActionComp->StartActionByName(this, "Fire");
	}
}

void ASGCharacterBase::StopFire()
{
	if (WeaponComp->CurrentWeapon != nullptr && bEquipWeapon)
	{
		ActionComp->StopActionByName(this, "Fire");
	}
}

void ASGCharacterBase::AimingActionStart_Implementation()
{
	// 蓝图
	ActionComp->StartActionByName(this, "Aiming");
}

void ASGCharacterBase::AimingActionStop_Implementation()
{
	// 蓝图
	ActionComp->StopActionByName(this, "Aiming");
}

void ASGCharacterBase::SwitchWeaponAction_Implementation()
{
	// 在蓝图中实现 - FixMe later
}

void ASGCharacterBase::EquipOrMountAction()
{
	if (WeaponComp->CurrentWeapon != nullptr)
	{
		if (bEquipWeapon)
		{
			bEquipWeapon = false;
			ChangeActionMode();
			PlayAnimMontage(EquipOrMountMontage, 2.0f);

			OnShowEquipmentStatus.Broadcast(bEquipWeapon, WeaponComp->CurrentWeapon);
		} else
		{
			bEquipWeapon = true;
			ChangeActionMode();
			PlayAnimMontage(EquipOrMountMontage, 2.0f);

			OnShowEquipmentStatus.Broadcast(bEquipWeapon, WeaponComp->CurrentWeapon);
		}
	}
}
// 由SGAnimNotify事件分发器触发
void ASGCharacterBase::SwitchWeapon()
{
	int32 WeaponMapNum = WeaponComp->WeaponMap.Num();
	if (WeaponMapNum > 1)
	{
		int32 SocketIndex = 1;
		int32 SocketArrayNum = WeaponComp->SocketArray.Num();
		// 查找当前武器的下一把武器对应挂载插槽的索引
		for (int32 i = 1; i < SocketArrayNum; i++)
		{
			FName SocketName = WeaponComp->SocketArray[i];
			if (WeaponComp->WeaponMap[SocketName] == WeaponComp->CurrentWeapon)
			{
				SocketIndex = i + 1;
				break;
			}
		}
		
		if (SocketIndex >= SocketArrayNum)
		{
			SocketIndex = 1;  // 注意第1个插槽为装备插槽
		}
		// 先挂载当前武器，再装备下一把武器
		FName SocketName = WeaponComp->SocketArray[SocketIndex];  
		WeaponComp->MountWeapon(this);
		WeaponComp->CurrentWeapon = WeaponComp->WeaponMap[SocketName];
		WeaponComp->EquipWeapon(this);

		OnShowEquipmentStatus.Broadcast(bEquipWeapon, WeaponComp->CurrentWeapon);
	}
}

void ASGCharacterBase::ReplaceClipAction()
{
	/*if (WeaponComp->CurrentWeapon != nullptr && bEquipWeapon)
	{
		WeaponComp->ReplaceClipCheck(this);
	}*/
	if (WeaponComp->CurrentWeapon != nullptr && bEquipWeapon)
	{
		ActionComp->StartActionByName(this, "Reload");
	}
}

void ASGCharacterBase::DiscardAction()
{
	if (WeaponComp->CurrentWeapon != nullptr && bEquipWeapon)
	{
		WeaponComp->DiscardWeapon(this);
	}
}

void ASGCharacterBase::TakeDirectDamage_Implementation(APawn* InstigatorPawn, const FHitResult& ImpactResult,
                                                       float Damage)
{
	float DamageThresholdScale = 1.0f;
	const UPhysicalMaterial* PhysicalMaterial = ImpactResult.PhysMaterial.Get();
	if (PhysicalMaterial != nullptr)
	{
		DamageThresholdScale = PhysicalMaterial->DestructibleDamageThresholdScale;
		UE_LOG(LogTemp, Log, TEXT("Direct Hit Physical Material is: %s"), *PhysicalMaterial->GetName());
	}
	AttributeComp->ApplyHealthChange(InstigatorPawn, -Damage * DamageThresholdScale);
}

void ASGCharacterBase::StopActionsFeedBack()
{
	StopFire();  // 停止开火 - 防止出现连发枪支继续开火的Bug
	AimingActionStop_Implementation();  // 停止瞄准
}



