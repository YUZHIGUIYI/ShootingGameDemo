// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SGShopAssistantCharacter.h"
#include "AI/SGShopAssistantController.h"
#include "AI/SGSmartObject.h"
#include "Camera/CameraComponent.h"

#define LOCTEXT_NAMESPACE "InteractableActors"

ASGShopAssistantCharacter::ASGShopAssistantCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp->SetupAttachment(RootComponent);

	// 确保AI控制器可以控制在世界中生成的AI
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	// 允许产生干涉事件 - 碰撞设置
	GetMesh()->SetGenerateOverlapEvents(true);
}

void ASGShopAssistantCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASGShopAssistantCharacter::Interact_Implementation(APawn* InstigatorPawn)
{
	if (InstigatorPawn == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Can not get InstigatorPawn"));
		return;
	}

	ASGShopAssistantController* ShopAssistantController = Cast<ASGShopAssistantController>(GetController());
	if (ShopAssistantController)
	{
		ShopAssistantController->ToggleShopMode(InstigatorPawn);
	}
}

FText ASGShopAssistantCharacter::GetInteractText_Implementation(APawn* InstigatorPawn)
{
	return LOCTEXT("Shop Message: ", "Consume credits in exanchange for weapons, ammo and medical kits.");
}

ASGSmartObject* ASGShopAssistantCharacter::GetSmartObject() const
{
	return SmartObject;
}

#undef LOCTEXT_NAMESPACE
