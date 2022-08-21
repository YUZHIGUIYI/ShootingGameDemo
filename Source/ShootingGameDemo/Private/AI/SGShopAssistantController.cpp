// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SGShopAssistantController.h"
#include "AI/SGShopAssistantCharacter.h"
#include "AI/SGSmartObject.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/Character.h"
#include "FunctionLibrary/SGGameplayFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"

ASGShopAssistantController::ASGShopAssistantController()
{
	BehaviorTreeComp = CreateDefaultSubobject<UBehaviorTreeComponent>("BehaviorTree");
}

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

void ASGShopAssistantController::BeginPlay()
{
	Super::BeginPlay();

	if (BehaviorTreeAsset)
	{
		RunBehaviorTree(BehaviorTreeAsset);
	}

	ControlledCharacter = Cast<ASGShopAssistantCharacter>(GetPawn());
	
	if (ControlledCharacter)
	{
		if (const ASGSmartObject* SmartObject = ControlledCharacter->GetSmartObject())
		{
			if (UBehaviorTree* Subtree = SmartObject->GetSubtree())
			{
				const FGameplayTag SubtreeTag;

				// 设置动态次级树
				// 在行为树中对应 Run Behavior Dynamic
				// 在运行时实时指定次级树
				BehaviorTreeComp->SetDynamicSubtree(SubtreeTag, Subtree);
			}
		}
	}
}
