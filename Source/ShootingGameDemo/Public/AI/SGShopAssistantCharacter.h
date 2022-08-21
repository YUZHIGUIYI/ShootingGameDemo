// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/SGGameplayInterface.h"
#include "SGShopAssistantCharacter.generated.h"

class UCameraComponent;
class ASGSmartObject;

UCLASS()
class SHOOTINGGAMEDEMO_API ASGShopAssistantCharacter : public ACharacter, public ISGGameplayInterface
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere)
	UCameraComponent* CameraComp;

	UPROPERTY(EditAnywhere)
	ASGSmartObject* SmartObject;
	
public:
	
	ASGShopAssistantCharacter();

protected:
	
	virtual void BeginPlay() override;

public:	

	virtual void Interact_Implementation(APawn* InstigatorPawn) override;

	virtual FText GetInteractText_Implementation(APawn* InstigatorPawn) override;

	ASGSmartObject* GetSmartObject() const;
};
