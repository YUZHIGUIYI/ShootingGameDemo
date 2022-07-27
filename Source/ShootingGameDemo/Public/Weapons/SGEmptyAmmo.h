// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SGEmptyAmmo.generated.h"

class UArrowComponent;
class USoundCue;

UCLASS()
class SHOOTINGGAMEDEMO_API ASGEmptyAmmo : public AActor
{
	GENERATED_BODY()
	
public:	
	
	ASGEmptyAmmo();

protected:

	UPROPERTY(EditDefaultsOnly, Category = "MassInKg")
	float MassInKg;

	UPROPERTY(EditDefaultsOnly, Category = "Impulse")
	float ImpulseParm;

	UPROPERTY(EditDefaultsOnly, Category = "LandingSound")
	USoundCue* LangdingSound;

	UPROPERTY(VisibleAnywhere, Category = "StaticMeshComp")
	UStaticMeshComponent* StaticMeshComp;

	UPROPERTY(VisibleAnywhere, Category = "ArrowComp")
	UArrowComponent* ArrowComp;
	
protected:

	UFUNCTION()
	void OnComponentHitHappened(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		FVector NormalImpulse, const FHitResult& Hit);
	
	virtual void BeginPlay() override;

public:	
	
	virtual void Tick(float DeltaTime) override;

};
