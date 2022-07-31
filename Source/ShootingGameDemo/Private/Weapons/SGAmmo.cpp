// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/SGAmmo.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Interface/SGDamageInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "PhysicalMaterials/PhysicalMaterial.h"

ASGAmmo::ASGAmmo()
{
	PrimaryActorTick.bCanEverTick = true;

	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	SphereComp->SetCollisionProfileName("Projectile");
	SphereComp->bReturnMaterialOnMove = true;  // 碰撞时返回被碰撞物的表面材质
	RootComponent = SphereComp;

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComp");
	StaticMeshComp->SetupAttachment(RootComponent);

	ParticleSystemComp = CreateDefaultSubobject<UParticleSystemComponent>("ParticleSystemComp");
	ParticleSystemComp->SetupAttachment(RootComponent);
	
	InitialSpeed = 10000.0f;
	MaxSpeed = 16000.0f;

	MoveComp = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMoveComp");
	MoveComp->bRotationFollowsVelocity = true;
	MoveComp->bInitialVelocityInLocalSpace = true;
	MoveComp->ProjectileGravityScale = 0.06f;
	MoveComp->InitialSpeed = InitialSpeed;
	MoveComp->MaxSpeed = MaxSpeed;
	
	DirectDamage = 45.0f;

	ImpactShakeInnerRadius = 0.0f;
	ImpactShakeOuterRadius = 1500.0f;

	AmmoType = EAmmoType::Sniper_ammo;

	//InitialLifeSpan = 2.0f;
}

void ASGAmmo::BeginPlay()
{
	Super::BeginPlay();
}

void ASGAmmo::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	MoveComp->OnProjectileStop.AddDynamic(this, &ASGAmmo::OnAmmoStop);
}

void ASGAmmo::OnAmmoStop(const FHitResult& ImpactResult)
{
	UE_LOG(LogTemp, Log, TEXT("Log On NormalAmmoStopFunction"));
	if (ImpactResult.bBlockingHit)
	{
		SpawnTM = FTransform(FRotator::ZeroRotator, ImpactResult.ImpactPoint);
		SpawnEmitterAtLocation(ImpactResult);
		PlaySoundAtLocation(ImpactResult);
		SpawnDecalAttached(ImpactResult);
		//PlayCameraShake();  // 效果不理想
		
		// 传递伤害测试
		AActor* HitActor = ImpactResult.GetActor();
		APawn* InstigatorPawn = GetInstigator();
		UE_LOG(LogTemp, Warning, TEXT("Instigator is: %s"), *GetNameSafe(InstigatorPawn));

		if (HitActor != InstigatorPawn)
		{
			if (HitActor->Implements<USGDamageInterface>())
			{
				ISGDamageInterface::Execute_TakeDirectDamage(HitActor, InstigatorPawn, ImpactResult, DirectDamage);
			}
		}
	}
	Destroy();
}

void ASGAmmo::SpawnEmitterAtLocation(const FHitResult& ImpactResult)
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), EffectInfo.MetalVFX, SpawnTM);
}

void ASGAmmo::PlaySoundAtLocation(const FHitResult& ImpactResult)
{
	const FVector ImpactPoint = ImpactResult.ImpactPoint;
	UPhysicalMaterial* PhysicalMaterial = ImpactResult.PhysMaterial.Get();
	if (PhysicalMaterial != nullptr)
	{
		// 需要事先在Build.cs中加入“PhysicsCore” 
		EPhysicalSurface SurfaceType = UPhysicalMaterial::DetermineSurfaceType(PhysicalMaterial);
		switch (SurfaceType)
		{
			case SurfaceType1:
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), EffectInfo.MetalSound, ImpactPoint);
				break;
			case SurfaceType2:
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), EffectInfo.BodySound, ImpactPoint);
				break;
			case SurfaceType3:
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), EffectInfo.BodySound, ImpactPoint);
				break;
			default:
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), EffectInfo.MetalSound, ImpactPoint);
				break;
		}
	} else
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), EffectInfo.MetalSound, ImpactPoint);
	}
}

void ASGAmmo::SpawnDecalAttached(const FHitResult& ImpactResult)
{
	if (EffectInfo.DamageDecal == nullptr)
	{
		return;
	}
	// 使用霰弹枪时有几率出现Ensure condition failed现象
	float Roll = FMath::RandRange(-90, 90);
	FRotator NormalRotator = ImpactResult.Normal.Rotation();  // @Fixme
	FRotator DecalRotation = FRotator(NormalRotator.Pitch, NormalRotator.Yaw, Roll);  // @Fixme
	FVector DecalSize = FVector(EffectInfo.DamageRegion, EffectInfo.DamageRegion, EffectInfo.DamageRegion);
	UGameplayStatics::SpawnDecalAttached(EffectInfo.DamageDecal, DecalSize, ImpactResult.GetComponent(), ImpactResult.BoneName,
		ImpactResult.ImpactPoint, DecalRotation, EAttachLocation::KeepWorldPosition, 2.0f);
}
// 弃用
void ASGAmmo::PlayCameraShake()
{
	if (ensure(ImpactShake))
	{
		UGameplayStatics::PlayWorldCameraShake(this, ImpactShake, GetActorLocation(),
			ImpactShakeInnerRadius, ImpactShakeOuterRadius);
		UE_LOG(LogTemp, Log, TEXT("CameraShake!"));
	}
}

void ASGAmmo::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

