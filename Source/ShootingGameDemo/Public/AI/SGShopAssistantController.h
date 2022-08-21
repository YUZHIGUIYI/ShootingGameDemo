// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "SGShopAssistantController.generated.h"

class UUserWidget;
class UBehaviorTreeComponent;
class ASGShopAssistantCharacter;
class UBehaviorTree;

UCLASS()
class SHOOTINGGAMEDEMO_API ASGShopAssistantController : public AAIController
{
	GENERATED_BODY()

protected:

	UPROPERTY()
	UUserWidget* ShopWidget;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> ShopWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "BehaviorTree")
	UBehaviorTree* BehaviorTreeAsset;
	
	UPROPERTY(EditDefaultsOnly, Category = "BehaviorTree")
	UBehaviorTreeComponent* BehaviorTreeComp;

	UPROPERTY()
	ASGShopAssistantCharacter* ControlledCharacter;

public:

	ASGShopAssistantController();
	
	void ToggleShopMode(APawn* InstigatorPawn);

protected:

	void BeginPlay() override;
};
