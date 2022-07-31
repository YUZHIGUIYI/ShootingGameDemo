// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SGShopAssistantController.h"
#include "AI/SGShopAssistantCharacter.h"
#include "Blueprint/UserWidget.h"
#include "Character/SGCharacterBase.h"
#include "Kismet/GameplayStatics.h"

void ASGShopAssistantController::EnterShopMode(APawn* InstigatorPawn)
{
	ASGCharacterBase* InstigatorCharacter = Cast<ASGCharacterBase>(InstigatorPawn);
	ASGShopAssistantCharacter* ShopAssistantCharacter = Cast<ASGShopAssistantCharacter>(GetPawn());
	if (InstigatorCharacter)
	{
		if (ShopWidget == nullptr)
		{
			ShopWidget = CreateWidget<UUserWidget>(GetWorld(), ShopWidgetClass); 
		}
		if (ShopWidget)
		{
			// 转换视角 - 单人游戏
			InstigatorCharacter->GetMesh()->SetVisibility(false);
			APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
			PlayerController->SetViewTargetWithBlend(ShopAssistantCharacter, 0.75f, EViewTargetBlendFunction::VTBlend_Cubic,
				2.0f);
			
			ShopWidget->AddToViewport(36);  // @Fixme later
			//ShopWidget->IsInViewport();

			FInputModeUIOnly InputModeUIOnly;
			InputModeUIOnly.SetWidgetToFocus(ShopWidget->TakeWidget());  // 转换成TSharedRef<SWidget>
			InputModeUIOnly.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			PlayerController->SetInputMode(InputModeUIOnly);
			PlayerController->bShowMouseCursor = true;

			UE_LOG(LogTemp, Warning, TEXT("Enter into shop!!!"));
		}
	}
}
