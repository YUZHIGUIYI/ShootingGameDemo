// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/SGBaseEffect.h"

ASGBaseEffect::ASGBaseEffect()
{
	PrimaryActorTick.bCanEverTick = true;

	ExistTime = 2.0f;
	DamageRegion = 20.0f;
}

void ASGBaseEffect::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASGBaseEffect::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

