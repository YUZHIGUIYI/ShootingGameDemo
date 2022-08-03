// Fill out your copyright notice in the Description page of Project Settings.


#include "GameSubsystems/SGSaveGameSubsystem.h"
#include "EngineUtils.h"
#include "GameFramework/GameStateBase.h"
#include "GameSubsystems/SGSaveGameSettings.h"
#include "Interface/SGGameplayInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Player/SGPlayerState.h"
#include "Player/SGSaveGame.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"

void USGSaveGameSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	const USGSaveGameSettings* SaveGameSettings = GetDefault<USGSaveGameSettings>();  // ??? - 获取SaveGame设置
	SlotName = SaveGameSettings->SaveGameSlotName;  // 从Default.ini中获取默认值
	
}

void USGSaveGameSubsystem::HandleRestorePlayerData(AController* NewPlayer)
{
	// 将序列化数据从PlayState导入到Player中 - 此函数在USGGameModeBase中调用
	ASGPlayerState* PlayerState = NewPlayer->GetPlayerState<ASGPlayerState>();
	if (ensure(PlayerState))
	{
		PlayerState->LoadPlayerState(CurrentSaveGame);
	}
}

bool USGSaveGameSubsystem::OverrideSpawnTransform(AController* NewPlayer)
{
	if (NewPlayer == nullptr || NewPlayer->IsPendingKill())
	{
		return false;
	}

	ASGPlayerState* PlayerState = NewPlayer->GetPlayerState<ASGPlayerState>();
	if (PlayerState == nullptr)
	{
		return false;
	}

	// 加载Player之前保存的位置信息
	if (APawn* PlayerPawn = PlayerState->GetPawn())
	{
		const FPlayerSaveData* PlayerSaveData = CurrentSaveGame->GetPlayerData(PlayerState);
		if (PlayerSaveData)
		{
			PlayerPawn->SetActorLocation(PlayerSaveData->Location);
			PlayerPawn->SetActorRotation(PlayerSaveData->Rotation);

			// PlayerState拥有者为Player Controller
			AController* PlayerController = Cast<AController>(PlayerState->GetOwner());  // 为何不用NewPlayer???
			//NewPlayer->SetControlRotation(PlayerSaveData->Rotation);
			PlayerController->SetControlRotation(PlayerSaveData->Rotation);  // 仅设置Pawn的旋转还不够，还需要设置控制旋转以改变摄像机的方向

			return true;
		}
	}

	return false;
}

void USGSaveGameSubsystem::ChangeSlotName(FString NewSlotName)
{
	// NewSlotName不应该为空 - 为空则不修改SaveGame插槽名称
	if (NewSlotName.Len() == 0)
	{
		return;
	}

	SlotName = NewSlotName;
}

void USGSaveGameSubsystem::WriteSaveGame()
{
	// 保存游戏状态
	// 首先将SavedPlayers与SavedActors数组置空
	CurrentSaveGame->SavedPlayers.Empty();
	CurrentSaveGame->SavedActors.Empty();

	AGameStateBase* GameState = GetWorld()->GetGameState();  // 获取游戏状态
	if (GameState == nullptr)
	{
		return;
	}

	// 迭代所有的PlayerState - 待改进
	for (int32 i = 0; i < GameState->PlayerArray.Num(); ++i)
	{
		ASGPlayerState* PlayerState = Cast<ASGPlayerState>(GameState->PlayerArray[i]);
		if (PlayerState)
		{
			PlayerState->SavePlayerState(CurrentSaveGame);  // 保存玩家的位置信息、积分
			break;  // 单一玩家
		}
	}

	// 迭代World中所有的Actor，保存其状态
	for (FActorIterator It(GetWorld()); It; ++It)
	{
		AActor* Actor = *It;

		// 仅保存满足条件的Actor - 使用IsPendingKill判断Actor是否有效，Actor必须被UPROPERTY()修饰
		// 当GC销毁Actor之后，Actor将会被设置为nullptr
		if (Actor->IsPendingKill() || !Actor->Implements<USGGameplayInterface>())
		{
			continue;;
		}

		FActorSaveData ActorSaveData;
		ActorSaveData.NewActorName = Actor->GetFName();
		ActorSaveData.Transform = Actor->GetActorTransform();

		// 传递存储
		FMemoryWriter MemWriter(ActorSaveData.ByteData);
		FObjectAndNameAsStringProxyArchive Ar(MemWriter, true);  // 代理归档，将UObject和FName序列化为字符串数据

		// 找到声明为UPROPERTY(SaveGame)的变量
		Ar.ArIsSaveGame = true;
		// 将Actor中声明为UPROPERTY()的SaveGame属性转换为二进制数组 - 序列化
		Actor->Serialize(Ar);

		CurrentSaveGame->SavedActors.Add(ActorSaveData);
	}

	UGameplayStatics::SaveGameToSlot(CurrentSaveGame, SlotName, 0);  // 保存游戏

	OnSaveGameWritten.Broadcast(CurrentSaveGame);  // 多播委托
}

void USGSaveGameSubsystem::LoadSaveGame(FString LoadedSlotName)
{
	// 载入游戏
	// 首先更新SaveGame插槽名
	ChangeSlotName(LoadedSlotName);

	if (UGameplayStatics::DoesSaveGameExist(SlotName, 0))
	{
		CurrentSaveGame = Cast<USGSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));
		if (CurrentSaveGame == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to load previous game data!"));
			return;
		}

		UE_LOG(LogTemp, Log, TEXT("Begin to load SaveGame data."));

		// 迭代World中的所有Actor
		for (FActorIterator It(GetWorld()); It; ++It)
		{
			AActor* Actor = *It;
			// 满足要求的Actor
			if (!Actor->Implements<USGGameplayInterface>())
			{
				continue;
			}

			for (FActorSaveData& ActorSaveData : CurrentSaveGame->SavedActors)
			{
				if (ActorSaveData.NewActorName == Actor->GetFName())
				{
					Actor->SetActorTransform(ActorSaveData.Transform);
					// 读取数据
					FMemoryReader MemReader(ActorSaveData.ByteData);

					FObjectAndNameAsStringProxyArchive Ar(MemReader, true);
					Ar.ArIsSaveGame = true;
					// 反序列化
					Actor->Serialize(Ar);

					// 待实现 - 接口实现函数

					break;;
				}
			}
		}

		OnSaveGameLoaded.Broadcast(CurrentSaveGame);
	} else
	{
		// 没有则创建新的SaveGame
		CurrentSaveGame = Cast<USGSaveGame>(UGameplayStatics::CreateSaveGameObject(USGSaveGame::StaticClass()));

		UE_LOG(LogTemp, Log, TEXT("Created New SaveGame Data."));
	}
}
