// Fill out your copyright notice in the Description page of Project Settings.


#include "ShootingGameDemo/Public/Player/SGPlayerController.h"

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
