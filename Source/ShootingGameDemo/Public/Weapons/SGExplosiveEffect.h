// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SGExplosiveEffect.generated.h"

class URadialForceComponent;

UCLASS()
class SHOOTINGGAMEDEMO_API ASGExplosiveEffect : public AActor
{
	GENERATED_BODY()

protected:

	UPROPERTY(VisibleAnywhere)
	URadialForceComponent* ForceComp;
	
public:	
	
	ASGExplosiveEffect();

protected:
	
	virtual void BeginPlay() override;

public:	

	virtual void Tick(float DeltaTime) override;

};
