// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/SGRocketAmmo.h"
#include "DrawDebugHelpers.h"
#include "Character/SGCharacterBase.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Interface/SGDamageInterface.h"
#include "Kismet/KismetSystemLibrary.h"

ASGRocketAmmo::ASGRocketAmmo()
{
	SphereComp->SetSphereRadius(20.0f);

	InitialSpeed = 1000.0f;
	MaxSpeed = 1700.0f;
	
	MoveComp->ProjectileGravityScale = 0.5f;
	MoveComp->InitialSpeed = InitialSpeed;
	MoveComp->MaxSpeed = MaxSpeed;

	ExplodeDamage = 125.0f;
	ExplodeRadius = 600.0f;
	ExplodeStrength = 10000.0f;

	HitObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));
	HitObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));
	HitObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));

	ActorsToIgnore.Add(this);
	
	MoveComp->OnProjectileStop.AddDynamic(this, &ASGRocketAmmo::OnProjectileStop);
}

void ASGRocketAmmo::OnProjectileStop(const FHitResult& ImpactResult)
{
	//Super::OnProjectileStop(ImpactResult);
	UE_LOG(LogTemp, Log, TEXT("Log on RocketProjectedFunction!"));
	if (ImpactResult.bBlockingHit)
	{
		FVector SpherePos = ImpactResult.ImpactPoint;
		float SphereRadius = ExplodeRadius;

		OutActors.Empty();  // ??? ASGCharacterBase::StaticClass()
		bool bIsOverlap = UKismetSystemLibrary::SphereOverlapActors(GetWorld(), SpherePos, SphereRadius, HitObjectTypes,
			nullptr, ActorsToIgnore, OutActors);

		SpawnTM = FTransform(FRotator::ZeroRotator, ImpactResult.ImpactPoint);

		SpawnEmitterAtLocation(ImpactResult);
		PlaySoundAtLocation(ImpactResult);
		SpawnDecalAttached(ImpactResult);
		//PlayCameraShake();
		
		if (bIsOverlap)
		{
			for (AActor* Actor : OutActors)
			{
				UE_LOG(LogTemp, Warning, TEXT("Explode: %s is included in OutActors!"), *GetNameSafe(Actor));
				TransmitExplosiveDamage(Actor, ImpactResult);  // 若实现了 SGDamageInterface接口则传递折减伤害
			}
			DrawDebugSphere(GetWorld(), SpherePos, SphereRadius, 32, FColor::Red, false, 5.0f);
		} else
		{
			UE_LOG(LogTemp, Warning, TEXT("Explode overlap failed!!!"));
		}
		
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = GetInstigator();
		GetWorld()->SpawnActor<AActor>(ExplosiveEffect, SpawnTM, SpawnParams);
	}
	Destroy();
}

void ASGRocketAmmo::SpawnEmitterAtLocation(const FHitResult& ImpactResult)
{
	Super::SpawnEmitterAtLocation(ImpactResult);
}

void ASGRocketAmmo::PlaySoundAtLocation(const FHitResult& ImpactResult)
{
	Super::PlaySoundAtLocation(ImpactResult);
}

void ASGRocketAmmo::PlayCameraShake()
{
	Super::PlayCameraShake();
}

void ASGRocketAmmo::TransmitExplosiveDamage(AActor* ActorInRange, const FHitResult& ImpactResult)
{
	APawn* InstigatorPawn = GetInstigator();
	if (ActorInRange != InstigatorPawn)
	{
		if (!ActorInRange->Implements<USGDamageInterface>())
		{
			return;
		}
		
		float DistanceBetween = FVector::Distance(ImpactResult.ImpactPoint, ActorInRange->GetActorLocation());
		DistanceBetween = FMath::Clamp(DistanceBetween, 0.0f, ExplodeRadius);
		float ReductionFactor = 1.0f - DistanceBetween / ExplodeRadius;
		ISGDamageInterface::Execute_TakeExplosiveDamage(ActorInRange, InstigatorPawn, ImpactResult,
			ExplodeDamage * ReductionFactor);
	}
}

