// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/SGGameModeBase.h"
#include "DrawDebugHelpers.h"
#include "AI/SGAICharacter.h"
#include "Character/SGAttributeComponent.h"
#include "EngineUtils.h"
#include "Blueprint/UserWidget.h"
#include "Character/SGActionComponent.h"
#include "Character/SGCharacterBase.h"
#include "Engine/AssetManager.h"
#include "EnvironmentQuery/EnvQueryInstanceBlueprintWrapper.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "GameMode/SGAIDataAsset.h"
#include "GameSubsystems/SGGameMissionSubsystem.h"
#include "GameSubsystems/SGSaveGameSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Player/SGPlayerController.h"
#include "Player/SGPlayerState.h"

// 控制台指令 - 控制AI生成
static TAutoConsoleVariable<bool> CVarSpawnAI(TEXT("su.SpawnAI"), true, TEXT("Enable spawning of AI via timer."),
	ECVF_Cheat);

ASGGameModeBase::ASGGameModeBase()
{
	CooldownBetweenFailure = 8.0f;
	SpawnTimeInterval = 2.5f;
	CreditsPerKill = 25.0f;
	ScorePerKill = 20;
	RequiredNumOfKills = 1;

	PlayerStateClass = ASGPlayerState::StaticClass();
}

int32 ASGGameModeBase::GetRequiredNumOfKills() const
{
	return RequiredNumOfKills;
}

void ASGGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	// 获取SGSaveGameSubsystem
	/*USGSaveGameSubsystem* SGSubsystem = GetGameInstance()->GetSubsystem<USGSaveGameSubsystem>();*/

	// 可选插槽名称，否则返回SGSaveGameSettings中指定的插槽
	/*FString SelectedSaveGameSlot = UGameplayStatics::ParseOption(Options, "SaveGame");
	SGSubsystem->LoadSaveGame(SelectedSaveGameSlot);*/
}

void ASGGameModeBase::StartPlay()
{
	Super::StartPlay();

	// 生成AI定时器
	GetWorldTimerManager().SetTimer(TimerHandle_SpawnAI, this, &ASGGameModeBase::SpawnAITimerElapsed,
		SpawnTimeInterval, true);
}

void ASGGameModeBase::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	// 在Super父类之前调用，在SGPlayerController中调用BeginPlayingState之前设置变量（该函数可实例化UI）
	/*USGSaveGameSubsystem* SGSubsystem = GetGameInstance()->GetSubsystem<USGSaveGameSubsystem>();
	SGSubsystem->HandleRestorePlayerData(NewPlayer);*/
	
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);

	// 覆盖Player的出生点
	/*SGSubsystem->OverrideSpawnTransform(NewPlayer);*/
}

void ASGGameModeBase::SpawnAITimerElapsed()
{
	// 在游戏线程中调用 - 若为true，停止产生AI
	if (!CVarSpawnAI.GetValueOnGameThread())
	{
		UE_LOG(LogTemp, Warning, TEXT("AI spawning disabled via Cvar 'CVarSpawnAI'."));
		return;
	}

	int32 NumOfAliveAI = 0;
	// 类似于Get all actors of class
	for (TActorIterator<ASGAICharacter> It(GetWorld()); It; ++It)
	{
		ASGAICharacter* AIPawn = *It;

		USGAttributeComponent* AttributeComp = USGAttributeComponent::GetAttributes(AIPawn);
		if (ensure(AttributeComp) && AttributeComp->IsAlive())
		{
			NumOfAliveAI++;
		}
	}

	UE_LOG(LogTemp, Log, TEXT("Found %d alive AI."), NumOfAliveAI);  // ??

	// 生成AI数量随时间的变化曲线
	float MaxAICount = 8.0f;
	if (SpawnAICurve)
	{
		MaxAICount = SpawnAICurve->GetFloatValue(GetWorld()->TimeSeconds);
	}
	if (NumOfAliveAI >= MaxAICount)
	{
		UE_LOG(LogTemp, Log, TEXT("Reached maximum AI capacity. Suspend AI spawn."));
		return;
	}

	UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this, SpawnAIQuery, this,
		EEnvQueryRunMode::RandomBest5Pct, nullptr);
	if (ensure(QueryInstance))
	{
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ASGGameModeBase::OnAISpawnQueryCompleted);
	}
}

void ASGGameModeBase::OnAISpawnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{
	if (QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn AI EQS Query Failed!"));
		return;
	}

	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();
	// 至少存在一个位置
	if (Locations.IsValidIndex(0))
	{
		// AI 数据表
		if (AIDataTable)
		{
			TArray<FAIInfoRow*> InfoRows;
			AIDataTable->GetAllRows("", InfoRows); // 获取所有行

			int32 RandomIndex = FMath::RandRange(0, InfoRows.Num() - 1);
			FAIInfoRow* SelectedRow = InfoRows[RandomIndex];  // 随机选取一行

			// 使用资产管理器 - 启动同步加载
			UAssetManager* AssetManager = UAssetManager::GetIfValid();
			if (AssetManager)
			{
				UE_LOG(LogTemp, Log, TEXT("Loading AI Asset..."));  // ??
				
				TArray<FName> Bundles;

				FStreamableDelegate Delegate =FStreamableDelegate::CreateUObject(this, &ASGGameModeBase::OnAIDataLoaded,
					SelectedRow->AIId, Locations[0]);

				AssetManager->LoadPrimaryAsset(SelectedRow->AIId, Bundles, Delegate);
			}
		}
		DrawDebugSphere(GetWorld(), Locations[0], 50.0f, 18,FColor::Purple, false, 25.0f);
	}
}

void ASGGameModeBase::OnAIDataLoaded(FPrimaryAssetId LoadedId, FVector SpawnLocation)
{
	UE_LOG(LogTemp, Log, TEXT("Finished loading!"));  // ??

	// 生成AI实例，赋予Action
	UAssetManager* AssetManager = UAssetManager::GetIfValid();
	if (AssetManager)
	{
		USGAIDataAsset* AIData = Cast<USGAIDataAsset>(AssetManager->GetPrimaryAssetObject(LoadedId));
		if (AIData)
		{
			AActor* NewAI = GetWorld()->SpawnActor<AActor>(AIData->AIClass, SpawnLocation, FRotator::ZeroRotator);
			if (NewAI)
			{
				UE_LOG(LogTemp, Log, TEXT("Spawn AI: %s (%s)"), *GetNameSafe(NewAI), *GetNameSafe(AIData));

				// 赋予AI Action
				USGActionComponent* ActionComp = Cast<USGActionComponent>(NewAI->GetComponentByClass(
					USGActionComponent::StaticClass()));
				if (ActionComp)
				{
					for (TSubclassOf<USGAction> ActionClass : AIData->AIActions)
					{
						ActionComp->AddAction(NewAI, ActionClass);
					}
				}
			}
		}
	}
}

void ASGGameModeBase::RespawnPlayerElapsed(AController* Controller)
{
	// Player重生
	if (ensure(Controller))
	{
		Controller->UnPossess();  // 移除Controller对Character的控制
		RestartPlayer(Controller);  // 在死亡处重生
	}
}

void ASGGameModeBase::OnActorKilled(AActor* VictimActor, AActor* Killer)
{
	ASGCharacterBase* Player = Cast<ASGCharacterBase>(VictimActor);

	if (Player)
	{
		// 多人？？？
		FTimerHandle TimerHandle_RespawnDelay;

		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "RespawnPlayerElapsed", Player->GetController());

		const float RespawnDelay = 4.0f;
		GetWorldTimerManager().SetTimer(TimerHandle_RespawnDelay, Delegate, RespawnDelay, false);

		USGGameMissionSubsystem* MissionSubsystem = GetGameInstance()->GetSubsystem<USGGameMissionSubsystem>();
		if (MissionSubsystem)
		{
			MissionSubsystem->AddScore(-ScorePerKill * 0.5);  // 死亡扣除分数
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("OnActorKilled: Victim: %s, Killer: %s"), *GetNameSafe(VictimActor), *GetNameSafe(Killer));

	// Player的击杀奖励
	// 通过APawn获取PlayerState
	APawn* KillerPawn = Cast<APawn>(Killer);
	if (KillerPawn)
	{
		ASGPlayerState* PlayerState = KillerPawn->GetPlayerState<ASGPlayerState>();  // 获取玩家的PlayerState
		if (PlayerState)
		{
			PlayerState->AddCredits(CreditsPerKill);
			PlayerState->AddNumOfKills(1);  // PlayerState或者Subsystem中？
			UE_LOG(LogTemp, Log, TEXT("Kill %d AI."), PlayerState->GetNumOfKills());
			
			USGGameMissionSubsystem* MissionSubsystem = GetGameInstance()->GetSubsystem<USGGameMissionSubsystem>();
			if (MissionSubsystem)
			{
				MissionSubsystem->AddScore(ScorePerKill);  // 击杀AI获得分数
				MissionSubsystem->AddNumOfKills(1);  // 击杀数量 + 1，不适合放在子系统中（多人无法兼顾），仅展示
			}
			
			if (PlayerState->GetNumOfKills() > RequiredNumOfKills)
			{
				CompleteMission(KillerPawn);
			}
		}
	}
}

void ASGGameModeBase::KillAllAI()
{
	for (TActorIterator<ASGAICharacter> It(GetWorld()); It; ++It)
	{
		ASGAICharacter* AICharacter = *It;

		USGAttributeComponent* AttributeComp = USGAttributeComponent::GetAttributes(AICharacter);
		if (ensure(AttributeComp) && AttributeComp->IsAlive())
		{
			AttributeComp->Kill(this);
		}
	}
}

void ASGGameModeBase::CompleteMission(APawn* Killer)
{
	if (Killer)
	{
		ASGPlayerController* PlayerController = Cast<ASGPlayerController>(Killer->GetController());
		if (PlayerController)
		{
			//Killer->DisableInput(PlayerController);  // 禁止输入事件
			GetWorldTimerManager().ClearTimer(TimerHandle_SpawnAI);  // 停止生成AI
			KillAllAI();  // 强制Kill所有AI
			UE_LOG(LogTemp, Log, TEXT("You Win!"));
			
			// 任务结束 - 待实现
			PlayerController->ToggleMissionSettlementWidget();
		}
	}
}

