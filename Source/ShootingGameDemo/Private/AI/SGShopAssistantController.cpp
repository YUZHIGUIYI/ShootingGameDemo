// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SGShopAssistantController.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/Character.h"
#include "FunctionLibrary/SGGameplayFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"

void ASGShopAssistantController::ToggleShopMode(APawn* InstigatorPawn)
{
	APawn* ControlledPawn = Cast<APawn>(GetPawn());
	ACharacter* InstigatorCharacter = Cast<ACharacter>(InstigatorPawn);
	
	if (InstigatorCharacter && ControlledPawn)
	{
		ShopWidget = CreateWidget<UUserWidget>(GetWorld(), ShopWidgetClass); 
		if (ShopWidget)
		{
			// 进入交互，视角转到可交互对象上，设置为UI模式
			USGGameplayFunctionLibrary::ShiftOfPerspectives(InstigatorCharacter, ControlledPawn, false);

			/*APlayerController* PlayerController = Cast<APlayerController>(InstigatorCharacter->GetController());
			UE_LOG(LogTemp, Log, TEXT("PlayerController is: %s!"), *GetNameSafe(PlayerController));
			FInputModeUIOnly InputModeUIOnly;
			InputModeUIOnly.SetWidgetToFocus(ShopWidget->TakeWidget());  // 转换成TSharedRef<SWidget>
			InputModeUIOnly.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			PlayerController->SetInputMode(InputModeUIOnly);
			PlayerController->bShowMouseCursor = true;*/

			ShopWidget->AddToViewport(36);  // @Fixme later
			UE_LOG(LogTemp, Warning, TEXT("Enter into shop!!!"));
		}
	}
}
