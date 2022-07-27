// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SGCharacterBase.h"
#include "Components/ActorComponent.h"
#include "SGInteractionComponent.generated.h"

class USGWorldWidget;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHOOTINGGAMEDEMO_API USGInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

protected:

	UPROPERTY()
	ASGCharacterBase* Player;  // 组件拥有者
	
	UPROPERTY()
	AActor* FocusedActor;

	UPROPERTY(EditDefaultsOnly, Category = "Trace")
	float TraceDistance;

	UPROPERTY(EditDefaultsOnly, Category = "Trace")
	float TraceRadius;

	UPROPERTY(EditDefaultsOnly, Category = "Trace")
	TEnumAsByte<ECollisionChannel> CollisionChannel;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<USGWorldWidget> DefaultWidgetClass;

	UPROPERTY()
	USGWorldWidget* DefaultWidgetInstance;
	
public:

	void PrimaryInteract();
	
	USGInteractionComponent();

protected:

	void FindBestInteractable();
	
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
};
