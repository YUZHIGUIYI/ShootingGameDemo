// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "SGShopAssistantController.generated.h"

class UUserWidget;

UCLASS()
class SHOOTINGGAMEDEMO_API ASGShopAssistantController : public AAIController
{
	GENERATED_BODY()

protected:

	UPROPERTY()
	UUserWidget* ShopWidget;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> ShopWidgetClass;

public:

	void EnterShopMode(APawn* InstigatorPawn);
};
