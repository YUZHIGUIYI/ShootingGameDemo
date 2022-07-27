// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/SGWeaponComponent.h"
#include "DrawDebugHelpers.h"
#include "Character/SGCharacterBase.h"
#include "Kismet/GameplayStatics.h"
#include "Weapons/SGWeaponBase.h"

USGWeaponComponent::USGWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	WeaponNum = 0;
	CurrentWeapon = nullptr;

	bCanFire = true;  // 控制单发武器的射击，达成类似于do once的效果

	for (int32 i = 1; i < SocketArray.Num(); i++)
	{
		WeaponMap.Emplace(SocketArray[i], nullptr);  // 仅挂载插槽
	}
}

USGWeaponComponent* USGWeaponComponent::GetWeaponComponent(AActor* FromActor)
{
	if (FromActor)
	{
		return Cast<USGWeaponComponent>(FromActor->GetComponentByClass(USGWeaponComponent::StaticClass()));
	}
	return nullptr;
}

bool USGWeaponComponent::EquipWeapon(ASGCharacterBase* InstigatorCharacter)
{
	// 将CurrentWeapon绑定到装备插槽上
	CurrentWeapon->GetSkeletalMesh()->AttachToComponent(InstigatorCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale,
			SocketArray[0]);
	UGameplayStatics::PlaySound2D(GetWorld(), CurrentWeapon->EquipSound);  // @Fixme;
	return true;
}

bool USGWeaponComponent::MountWeapon(ASGCharacterBase* InstigatorCharacter)
{
	for (int32 i = 1; i < SocketArray.Num(); i++)
	{
		// 查找CurrentWeapon的挂载插槽
		FName SocketName = SocketArray[i];
		if (WeaponMap[SocketName] == CurrentWeapon)
		{
			CurrentWeapon->GetSkeletalMesh()->AttachToComponent(InstigatorCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale,
				SocketName);
			UGameplayStatics::PlaySound2D(GetWorld(), CurrentWeapon->EquipSound);  // @fixme
			return true;
		}
	}
	return false;
}

void USGWeaponComponent::ReplaceClipCheck(ASGCharacterBase* InstigatorCharacter)
{
	if (!(InstigatorCharacter->bEquipWeapon) || (CurrentWeapon == nullptr))
	{
		UE_LOG(LogTemp, Log, TEXT("Without weapon, can not reload clip."));
		return;
	}
	if (CurrentWeapon->MaxPrimaryClipAmmo <= 0)
	{
		UE_LOG(LogTemp, Log, TEXT("Clip is empty!"))
		UGameplayStatics::PlaySound2D(GetWorld(), CurrentWeapon->EmptyClipSound);
		return;
	} else if (CurrentWeapon->PrimaryClipAmmo == CurrentWeapon->ClipLoad)
	{
		UE_LOG(LogTemp, Log, TEXT("Clip is full! No need to replace clip!"));
		return;
	} else
	{
		InstigatorCharacter->PlayAnimMontage(InstigatorCharacter->ReplaceClipMontage);
		CurrentWeapon->GetSkeletalMesh()->PlayAnimation(CurrentWeapon->ReloadSequence, false);
		//ReplaceClipBegin(InstigatorCharacter);
	}
}

// 由Character换弹蒙太奇调用
void USGWeaponComponent::ReplaceClipBegin(ASGCharacterBase* InstigatorCharacter)
{
	int32 NeededAmmo = CurrentWeapon->ClipLoad - CurrentWeapon->PrimaryClipAmmo;
	if (CurrentWeapon->MaxPrimaryClipAmmo <= NeededAmmo)
	{
		CurrentWeapon->PrimaryClipAmmo = CurrentWeapon->PrimaryClipAmmo + CurrentWeapon->MaxPrimaryClipAmmo;
		CurrentWeapon->MaxPrimaryClipAmmo = 0;
	} else
	{
		CurrentWeapon->PrimaryClipAmmo = CurrentWeapon->ClipLoad;
		CurrentWeapon->MaxPrimaryClipAmmo = CurrentWeapon->MaxPrimaryClipAmmo - NeededAmmo;
	}
	
	OnClipStatusChanged.Broadcast(this, CurrentWeapon->PrimaryClipAmmo, CurrentWeapon->MaxPrimaryClipAmmo);  // 多播委托
}

void USGWeaponComponent::DiscardWeapon(ASGCharacterBase* InstigatorCharacter)
{
	if (CurrentWeapon && InstigatorCharacter->bEquipWeapon)
	{
		// 从Actor中分离 - 模拟物理 - 开启碰撞 - 施加冲量
		CurrentWeapon->GetSkeletalMesh()->SetSimulatePhysics(true);
		CurrentWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		CurrentWeapon->GetSkeletalMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		CurrentWeapon->GetSkeletalMesh()->AddImpulse(InstigatorCharacter->GetActorForwardVector() * 1000.0f);
		// 将CurrentMap对应的TMap中的指针置空
		for (int32 i = 1; i < SocketArray.Num(); i++)
		{
			if (CurrentWeapon == WeaponMap[SocketArray[i]])
			{
				WeaponMap[SocketArray[i]] = nullptr;
				CurrentWeapon = nullptr;
				WeaponNum--;
				
				InstigatorCharacter->bEquipWeapon = false;
				InstigatorCharacter->ChangeActionMode();
				InstigatorCharacter->OnShowEquipmentStatus.Broadcast(InstigatorCharacter->bEquipWeapon, nullptr);

				UE_LOG(LogTemp, Warning, TEXT("Discard current weapon!"));
				
				break;
			}
		}
		// 查找是否存在可用的武器
		for (int32 i = 1; i < SocketArray.Num(); i++)
		{
			if (WeaponMap[SocketArray[i]] != nullptr)
			{
				CurrentWeapon = WeaponMap[SocketArray[i]];
				break;
			}
		}
	}
}

void USGWeaponComponent::DiscardAllWeapons(ASGCharacterBase* InstigatorCharacter)
{
	// Character死亡时掉落所有武器
	for (int32 i = 1; i < SocketArray.Num(); i++)
	{
		if (WeaponMap[SocketArray[i]] != nullptr)
		{
			WeaponMap[SocketArray[i]]->GetSkeletalMesh()->SetSimulatePhysics(true);
			WeaponMap[SocketArray[i]]->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
			WeaponMap[SocketArray[i]]->GetSkeletalMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			WeaponMap[SocketArray[i]]->GetSkeletalMesh()->AddImpulse(InstigatorCharacter->GetActorUpVector() * 800.0f);
			WeaponNum--;
			WeaponMap[SocketArray[i]] = nullptr;
		}
	}
}

void USGWeaponComponent::StartFireAction(ASGCharacterBase* InstigatorCharacter)
{
	if (!(InstigatorCharacter->bEquipWeapon) || (CurrentWeapon == nullptr))
	{
		UE_LOG(LogTemp, Log, TEXT("Without weapon, can not fire."));
		return;
	}
	if (CurrentWeapon->PrimaryClipAmmo <= 0)
	{
		UE_LOG(LogTemp, Log, TEXT("Firearm's clip is empty, please replace clip!"));
		return;
	}
	const float ShootingTime = CurrentWeapon->ShootingSpeed;
	if (CurrentWeapon->ShootingType == EShootingType::Running)
	{
		//FTimerHandle TimerHandle_FireDelay;  // 局部
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "StartFireDelay_Elapsed", InstigatorCharacter);
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_FireDelay, Delegate, ShootingTime, true);
	} else
	{
		if (bCanFire)
		{
			UE_LOG(LogTemp, Log, TEXT("Single Shooting!"));
			bCanFire = false;
			StartFireDelay_Elapsed(InstigatorCharacter);
			const FLatentActionInfo LatentInfo(0, FMath::Rand(), TEXT("OnDelayFinished"), this);
			UKismetSystemLibrary::Delay(this, ShootingTime, LatentInfo);  // @Fixme: 使用Delay方法是否合适？
		}
	}
}

void USGWeaponComponent::StopFireAction(ASGCharacterBase* InstigatorCharacter)
{
	if (CurrentWeapon->WeaponType == EWepaonType::Rifle && InstigatorCharacter->bEquipWeapon)
	{
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle_FireDelay);
	}
}

void USGWeaponComponent::OnDelayFinished()
{
	UKismetSystemLibrary::PrintString(this, FString("After Delay!"));
	bCanFire = true;
}

void USGWeaponComponent::StartFireDelay_Elapsed(ASGCharacterBase* InstigatorCharacter)  //@fixme
{
	if (CurrentWeapon->WeaponType == EWepaonType::Rifle && CurrentWeapon->PrimaryClipAmmo <= 0)
	{
		UE_LOG(LogTemp, Log, TEXT("Firearm's clip is empty, please replace clip!"));
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle_FireDelay);
		return;  // @Fix me
	}
	
	if (CurrentWeapon->ShootingType == EShootingType::Running)
	{
		InstigatorCharacter->PlayAnimMontage(InstigatorCharacter->FireMontageNormal, 0.8);  // Character开火动画蒙太奇
	} else
	{
		InstigatorCharacter->PlayAnimMontage(InstigatorCharacter->FireMontageShotgun, 0.8);
	}

	//InstigatorCharacter->RecoilForce();  // 调用蓝图的CameraShake
	InstigatorCharacter->BeginRecoil();  // 测试用
	
	CurrentWeapon->GetSkeletalMesh()->PlayAnimation(CurrentWeapon->FireSequence, false);  // Weapon开火序列

	CurrentWeapon->PrimaryClipAmmo--;  // 子弹数量减1

	OnClipStatusChanged.Broadcast(this, CurrentWeapon->PrimaryClipAmmo, CurrentWeapon->MaxPrimaryClipAmmo);  // 多播委托
	
	FVector MuzzleLocation = CurrentWeapon->GetSkeletalMesh()->GetSocketLocation(CurrentWeapon->MuzzleSocketName);

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Instigator = InstigatorCharacter;

	FCollisionShape Shape;
	Shape.SetSphere(3.0f);

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(InstigatorCharacter);

	FCollisionObjectQueryParams ObjParams;  // @Fixme: 在头文件声明？
	ObjParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	ObjParams.AddObjectTypesToQuery(ECC_WorldStatic);
	ObjParams.AddObjectTypesToQuery(ECC_Pawn);

	FVector TraceStart = InstigatorCharacter->GetPawnViewLocation();  // 摄像机组件位置
	FVector TraceEnd = TraceStart + (InstigatorCharacter->GetControlRotation().Vector() * CurrentWeapon->ShootingDistance);
	//FVector TraceEnd = TraceStart + (InstigatorCharacter->GetCameraForwardVector() * CurrentWeapon->ShootingDistance);  // @Fix: 似乎有偏移
	FHitResult Hit;
	if (CurrentWeapon->BallisticType == EBallisticType::Projectile)
	{
		// 抛射型武器 - 狙击枪&来福枪&火箭筒&榴弹枪
		if (GetWorld()->SweepSingleByObjectType(Hit, TraceStart, TraceEnd, FQuat::Identity, ObjParams, Shape, Params))
		{
			TraceEnd = Hit.ImpactPoint;
		}

		FRotator ProjRotation = FRotationMatrix::MakeFromX(TraceEnd - MuzzleLocation).Rotator();

		FTransform SpawnTM = FTransform(ProjRotation, MuzzleLocation);
		//DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Green, false, 2.0f, 0, 0.5f);
		GetWorld()->SpawnActor<AActor>(CurrentWeapon->AmmoType, SpawnTM, SpawnParams);
	}
	else
	{
		// 霰弹枪
		for (int32 i = 0; i < 6; i++)
		{
			Hit.Init();
			float RandValue = FMath::RandRange(-CurrentWeapon->RecoilOffset, CurrentWeapon->RecoilOffset);
			TraceEnd.Y += RandValue;
			TraceEnd.Z += RandValue;
			
			if (GetWorld()->SweepSingleByObjectType(Hit, TraceStart, TraceEnd, FQuat::Identity, ObjParams, Shape, Params))
			{
				TraceEnd = Hit.ImpactPoint;
			}

			FRotator ProjRotation = FRotationMatrix::MakeFromX(TraceEnd - MuzzleLocation).Rotator();

			FTransform SpawnTM = FTransform(ProjRotation, MuzzleLocation);
			GetWorld()->SpawnActor<AActor>(CurrentWeapon->AmmoType, SpawnTM, SpawnParams);
		}
	}
	// 抛出空弹壳
	if (CurrentWeapon->WeaponType == EWepaonType::Grenade_gun || CurrentWeapon->WeaponType == EWepaonType::Rocket_launcher)
	{
		return;
	}
	if (!ensure(CurrentWeapon->EmptyAmmoType)) return;
	FVector ChamberLocation = CurrentWeapon->GetSkeletalMesh()->GetSocketLocation(CurrentWeapon->ChamberSocketName);
	GetWorld()->SpawnActor<AActor>(CurrentWeapon->EmptyAmmoType, ChamberLocation, FRotator::ZeroRotator, SpawnParams);
}

void USGWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

