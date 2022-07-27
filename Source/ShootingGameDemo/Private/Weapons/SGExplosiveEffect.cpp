// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/SGExplosiveEffect.h"
#include "PhysicsEngine/RadialForceComponent.h"


ASGExplosiveEffect::ASGExplosiveEffect()
{
	PrimaryActorTick.bCanEverTick = true;

	ForceComp = CreateDefaultSubobject<URadialForceComponent>("ForceComp");
	ForceComp->SetupAttachment(RootComponent);

	ForceComp->SetAutoActivate(true);  // 自动触发
	ForceComp->bImpulseVelChange = true;  // 不考虑质量影响
	ForceComp->Radius = 600.0f;
	ForceComp->ImpulseStrength = 2500.0f;
	ForceComp->AddCollisionChannelToAffect(ECC_Pawn);
	ForceComp->AddCollisionChannelToAffect(ECC_WorldDynamic);
}

void ASGExplosiveEffect::BeginPlay()
{
	Super::BeginPlay();

	ForceComp->FireImpulse();
	
	UE_LOG(LogTemp, Log, TEXT("Trigger Explosive event!"));
	
	SetLifeSpan(0.5f);
}

void ASGExplosiveEffect::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

