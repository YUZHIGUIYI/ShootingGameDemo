// Fill out your copyright notice in the Description page of Project Settings.


#include "ShootingGameDemo/Public/Player/SGPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

void ASGPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	// 处理输入组件 - 绑定暂停菜单按键 && 绑定背包UI按键
	InputComponent->BindAction("PauseGame", IE_Pressed, this, &ASGPlayerController::TogglePauseMenu);
	InputComponent->BindAction("Backpack", IE_Pressed, this, &ASGPlayerController::ToggleBackpackWidget);
}

void ASGPlayerController::TogglePauseMenu()
{
	// 第一次按键输入将呼出暂停菜单，游戏暂停；第二次按键输入将移除暂停菜单，游戏继续
	if (PauseMenuInstance && PauseMenuInstance->IsInViewport())
	{
		PauseMenuInstance->RemoveFromParent();
		PauseMenuInstance = nullptr;

		bShowMouseCursor = false;
		SetInputMode(FInputModeGameOnly());

		if (GetWorld()->IsNetMode(NM_Standalone))
		{
			UGameplayStatics::SetGamePaused(GetWorld(), false);
		}

		return;
	}
	// 第一次按键输入处理
	PauseMenuInstance = CreateWidget<UUserWidget>(this, PauseMenuClass);
	if (PauseMenuInstance)
	{
		PauseMenuInstance->AddToViewport(100);

		bShowMouseCursor = true;
		SetInputMode(FInputModeUIOnly());

		if (GetWorld()->IsNetMode(NM_Standalone))
		{
			UGameplayStatics::SetGamePaused(GetWorld(), true);  // 单人模式可暂停游戏
		}
	}
}

void ASGPlayerController::ToggleBackpackWidget()
{
	// 待实现
}

void ASGPlayerController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);

	OnPawnChanged.Broadcast(InPawn);  // 可以在Widget蓝图中分配，及时更新UI
}

void ASGPlayerController::BeginPlayingState()
{
	BlueprintBeginPlayingState();  // 确保PlayerState已经实例化
}

void ASGPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	OnPlayerStateReceived.Broadcast(PlayerState);  // 见Controller.h
}
