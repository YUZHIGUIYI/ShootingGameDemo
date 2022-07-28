// Fill out your copyright notice in the Description page of Project Settings.


#include "Actions/SGAction_Fire.h"
#include "Character/SGCharacterBase.h"
#include "Character/SGWeaponComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Weapons/SGWeaponBase.h"

USGAction_Fire::USGAction_Fire()
{
	bCanFire = true;

	ObjParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	ObjParams.AddObjectTypesToQuery(ECC_WorldStatic);
	ObjParams.AddObjectTypesToQuery(ECC_Pawn);

	Shape.SetSphere(3.0f);
}

void USGAction_Fire::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	ASGCharacterBase* InstigatorCharacter = Cast<ASGCharacterBase>(Instigator);
	if (!IsValid(InstigatorCharacter))
	{
		return;
	}
	
	WeaponComp = USGWeaponComponent::GetWeaponComponent(Instigator);
	CurrentWeapon = WeaponComp->CurrentWeapon;
	
	if (CurrentWeapon->PrimaryClipAmmo <= 0)
	{
		UE_LOG(LogTemp, Log, TEXT("Firearm's clip is empty, please replace clip!"));
		return;
	}

	const float ShootingTime = CurrentWeapon->ShootingSpeed;
	if (CurrentWeapon->ShootingType == EShootingType::Running)
	{
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "StartFireDelay_Elapsed", InstigatorCharacter);
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_FireDelay, Delegate, ShootingTime, true);
	} else
	{
		if (bCanFire)
		{
			UE_LOG(LogTemp, Log, TEXT("Single Shooting start!"));
			bCanFire = false;
			StartFireDelay_Elapsed(InstigatorCharacter);
			const FLatentActionInfo LatentInfo(0, FMath::Rand(), TEXT("OnDelayFinished"), this);
			UKismetSystemLibrary::Delay(this, ShootingTime, LatentInfo);  // @Fixme: 使用Delay方法是否合适？
		}
	}
}

void USGAction_Fire::StopFireAction()
{
	if (CurrentWeapon->WeaponType == EWepaonType::Rifle && CurrentWeapon->PrimaryClipAmmo > 0)
	{
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle_FireDelay);
	}
}

void USGAction_Fire::OnDelayFinished()
{
	UKismetSystemLibrary::PrintString(this, FString("Single Shooting After Delay!"));
	bCanFire = true;
}

void USGAction_Fire::StartFireDelay_Elapsed(ASGCharacterBase* InstigatorCharacter)
{
	if (CurrentWeapon->WeaponType == EWepaonType::Rifle && CurrentWeapon->PrimaryClipAmmo <= 0)
	{
		UE_LOG(LogTemp, Log, TEXT("Firearm's clip is empty, please replace clip!"));
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle_FireDelay);
		return;
	}

	if (CurrentWeapon->ShootingType == EShootingType::Running)
	{
		InstigatorCharacter->PlayAnimMontage(FireMontageNormal);   // 持连发枪支开火动画蒙太奇
	} else
	{
		InstigatorCharacter->PlayAnimMontage(FireMontageViolent);  // 持单发枪支开火动画蒙太奇
	}

	InstigatorCharacter->BeginRecoil();  // 后座力

	CurrentWeapon->GetSkeletalMesh()->PlayAnimation(CurrentWeapon->FireSequence, false);  // 枪支开火动画序列

	CurrentWeapon->PrimaryClipAmmo--;  // 子弹数量减1

	WeaponComp->OnClipStatusChanged.Broadcast(WeaponComp, CurrentWeapon->PrimaryClipAmmo, CurrentWeapon->MaxPrimaryClipAmmo);  // 枪支状态变化多播委托

	FVector MuzzleLocation = CurrentWeapon->GetSkeletalMesh()->GetSocketLocation(CurrentWeapon->MuzzleSocketName);  // 枪口位置

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Instigator = InstigatorCharacter;
	
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(InstigatorCharacter);

	const FVector TraceStart = InstigatorCharacter->GetPawnViewLocation();  // 射线起始位置 - 摄像机组件位置
	FVector TraceEnd = TraceStart + (InstigatorCharacter->GetControlRotation().Vector() * CurrentWeapon->ShootingDistance);  // 视线终点位置

	FHitResult Hit;
	if (CurrentWeapon->BallisticType == EBallisticType::Projectile)
	{
		// 抛射型武器 - 狙击枪&来福枪&火箭筒&榴弹枪
		if (GetWorld()->SweepSingleByObjectType(Hit, TraceStart, TraceEnd, FQuat::Identity, ObjParams, Shape, QueryParams))
		{
			TraceEnd = Hit.ImpactPoint;
		}

		FRotator ProjRotation = FRotationMatrix::MakeFromX(TraceEnd - MuzzleLocation).Rotator();

		FTransform SpawnTM = FTransform(ProjRotation, MuzzleLocation);

		GetWorld()->SpawnActor<AActor>(CurrentWeapon->AmmoType, SpawnTM, SpawnParams);
	} else
	{
		// 霰弹枪
		for (int32 i = 0; i < 6; i++)
		{
			Hit.Init();
			float RandValueY = FMath::RandRange(-CurrentWeapon->RecoilOffset, CurrentWeapon->RecoilOffset);
			float RandValueZ = FMath::RandRange(-CurrentWeapon->RecoilOffset, CurrentWeapon->RecoilOffset);
			TraceEnd.Y += RandValueY;
			TraceEnd.Z += RandValueZ;
			
			if (GetWorld()->SweepSingleByObjectType(Hit, TraceStart, TraceEnd, FQuat::Identity, ObjParams, Shape, QueryParams))
			{
				TraceEnd = Hit.ImpactPoint;
			}

			FRotator ProjRotation = FRotationMatrix::MakeFromX(TraceEnd - MuzzleLocation).Rotator();

			FTransform SpawnTM = FTransform(ProjRotation, MuzzleLocation);
			
			GetWorld()->SpawnActor<AActor>(CurrentWeapon->AmmoType, SpawnTM, SpawnParams);
		}
	}

	// 抛射空弹壳
	SpawnEmptyAmmo(SpawnParams);
}

void USGAction_Fire::SpawnEmptyAmmo(FActorSpawnParameters& SpawnParams)
{
	if (CurrentWeapon->WeaponType == EWepaonType::Grenade_gun || CurrentWeapon->WeaponType == EWepaonType::Rocket_launcher)
	{
		return;
	}
	
	if (ensure(CurrentWeapon->EmptyAmmoType))
	{
		FVector ChamberLocation = CurrentWeapon->GetSkeletalMesh()->GetSocketLocation(CurrentWeapon->ChamberSocketName);
		GetWorld()->SpawnActor<AActor>(CurrentWeapon->EmptyAmmoType, ChamberLocation, FRotator::ZeroRotator, SpawnParams);
	}
}

void USGAction_Fire::StopAction_Implementation(AActor* Instigator)
{
	StopFireAction();  // 如果是连发武器，则清除定时器句柄
	
	Super::StopAction_Implementation(Instigator);
}