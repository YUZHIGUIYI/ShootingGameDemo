// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/SGWeaponBase.h"
#include "Character/SGCharacterBase.h"
#include "Character/SGWeaponComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

#define LOCTEXT_NAMESPACE "InteractableActors"

ASGWeaponBase::ASGWeaponBase()
{
	PrimaryActorTick.bCanEverTick = true;

	SkeletalMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMesh");
	SkeletalMeshComp->SetCollisionProfileName("Weapon");
	RootComponent = SkeletalMeshComp;

	ShootingDistance = 5000.0f;
	ShootingSpeed = 2.0f;
	RecoilOffset = 27.0f;
	ChamberSocketName = "ChamberSocket";
	AttachedSocketName = "EquipSocket";
	MuzzleSocketName = "MuzzleFlash";
}

USkeletalMeshComponent* ASGWeaponBase::GetSkeletalMesh() const
{
	return SkeletalMeshComp;
}

int32 ASGWeaponBase::GetPrimaryClipAmmo() const
{
	return PrimaryClipAmmo;
}

int32 ASGWeaponBase::GetMaxPrimaryClipAmmo() const
{
	return MaxPrimaryClipAmmo;
}

void ASGWeaponBase::SetPrimaryClipAmmo(int32 NewPrimaryClipAmmo)
{
	int32 OldPrimaryClipAmmo = PrimaryClipAmmo;
	PrimaryClipAmmo = NewPrimaryClipAmmo;
	OnPrimaryClipAmmoChanged.Broadcast(OldPrimaryClipAmmo, PrimaryClipAmmo);
}

void ASGWeaponBase::SetMaxPrimaryClipAmmo(int32 NewMaxPrimaryClipAmmo)
{
	int32 OldMaxPrimaryClipAmmo = MaxPrimaryClipAmmo;
	MaxPrimaryClipAmmo = NewMaxPrimaryClipAmmo;
	OnMaxPrimaryClipAmmoChanged.Broadcast(OldMaxPrimaryClipAmmo, MaxPrimaryClipAmmo);
}

void ASGWeaponBase::Interact_Implementation(APawn* InstigatorPawn)
{
	if (InstigatorPawn == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Can not get InstigatorPawn"));
		return;
	}

	ASGCharacterBase* Character = Cast<ASGCharacterBase>(InstigatorPawn);
	USGWeaponComponent* WeaponComp = USGWeaponComponent::GetWeaponComponent(Character);  // 调用静态函数获取Weapon组件
	if ((!Character->bEquipWeapon) && (WeaponComp->CurrentWeapon == nullptr))
	{
		AttachedSocketName = WeaponComp->SocketArray[0];  // 索引0为装备插槽，不而非挂载插槽
		EquipOrMountWeapon(Character);
		UGameplayStatics::PlaySound2D(GetWorld(), EquipSound);
		WeaponComp->CurrentWeapon = this;
		WeaponComp->WeaponMap.Emplace(WeaponComp->SocketArray[1], this);  // 加入映射
		WeaponComp->WeaponNum += 1;
		
		Character->bEquipWeapon = true;
		Character->ChangeActionMode();
		Character->OnShowEquipmentStatus.Broadcast(Character->bEquipWeapon, this);
	} else
	{
		bool bEquipMount = false;
		// 遍历挂载插槽
		for (int32 i = 1; i < WeaponComp->SocketArray.Num(); i++)
		{
			FName SocketName = WeaponComp->SocketArray[i];
			if (WeaponComp->WeaponMap[SocketName] == nullptr)
			{
				AttachedSocketName = SocketName;
				bEquipMount = EquipOrMountWeapon(Character);
				UGameplayStatics::PlaySound2D(GetWorld(), PickupSound);
				WeaponComp->WeaponMap.Emplace(AttachedSocketName, this);  // 加入映射
				WeaponComp->WeaponNum += 1;
				break;
			}
		}
		if (!bEquipMount)
		{
			FString FailedMsg = FString::Printf(TEXT("Failed to equip or mount: %s, now has %d weapons"), *GetNameSafe(this), WeaponComp->WeaponMap.Num() + 1);
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FailedMsg);
		}
	}
	
}

bool ASGWeaponBase::EquipOrMountWeapon(ASGCharacterBase* Character)
{
	SkeletalMeshComp->SetSimulatePhysics(false);
	SkeletalMeshComp->AttachToComponent(Character->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, AttachedSocketName);
	SkeletalMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	/*if (!Character->bEquipWeapon)
	{
		Character->bEquipWeapon = true;
		Character->ChangeActionMode();
	}*/
	return true;
}

FText ASGWeaponBase::GetInteractText_Implementation(APawn* InstigatorPawn)
{
	//return FText::GetEmpty();
	//return LOCTEXT("Weapon Tips: ", "Can be attached!");
	return FText::Format(LOCTEXT("Weapon Message: ", "{0} x {1} Ammo"),WeaponName, (PrimaryClipAmmo + MaxPrimaryClipAmmo));
}

#undef LOCTEXT_NAMESPACE

