// Fill out your copyright notice in the Description page of Project Settings.


#include "ShootingGameDemo/Public/Player/SGPlayerState.h"
#include "Engine/AssetManager.h"
#include "FunctionLibrary/SGGameplayFunctionLibrary.h"
#include "Player/SGSaveGame.h"

void ASGPlayerState::AddCredits(int32 Delta)
{
	if (ensure(Delta >= 0.0f))
	{
		Credits += Delta;
		OnCreditsChanged.Broadcast(this, Credits, Delta);
	}
}

bool ASGPlayerState::RemoveCredits(int32 Delta)
{
	if (!ensure(Delta >= 0.0f))
	{
		return false;
	}
	if (Credits < Delta)
	{
		return false;
	}

	Credits -= Delta;

	OnCreditsChanged.Broadcast(this, Credits, -Delta);

	return true;
}

int32 ASGPlayerState::GetNumOfKills() const
{
	return NumOfKills;
}

void ASGPlayerState::AddNumOfKills(int32 Delta)
{
	if (ensure(Delta > 0))
	{
		NumOfKills += Delta;
		// 待实现
	}
}

void ASGPlayerState::SavePlayerState_Implementation(USGSaveGame* SaveObject)
{
	if (SaveObject)
	{
		// 将PlayerState中的数据存入SGSaveGame中的SavedPlayers中
		FPlayerSaveData SavedData;
		SavedData.Credits = Credits;
		SavedData.PersonalRecordTime = PersonalRecordTime;
		SavedData.PlayerID = GetUniqueId().ToString();  // 获取唯一的PlayerId，转换为FString

		// PlayerCharacter是否存活
		if (const APawn* PlayerPawn = GetPawn())
		{
			SavedData.Location = PlayerPawn->GetActorLocation();
			SavedData.Rotation = PlayerPawn->GetActorRotation();
			//  ??? 待实现
		}

		SaveObject->SavedPlayers.Add(SavedData);
	}
}

void ASGPlayerState::LoadPlayerState_Implementation(USGSaveGame* SaveObject)
{
	if (SaveObject)
	{
		FPlayerSaveData* FoundPlayerSaveData = SaveObject->GetPlayerData(this);
		if (FoundPlayerSaveData)
		{
			AddCredits(FoundPlayerSaveData->Credits);
			PersonalRecordTime = FoundPlayerSaveData->PersonalRecordTime;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Could not find SaveGame Data for player id: '%i'."), GetPlayerId());
		}
	}
}

bool ASGPlayerState::PurchaseAndAddToBackpack(FName RowName, int32 ObjectNum, bool IsGoods)
{
	if (ItemDataTable)
	{
		const FItemInfoRowNew* SelectedRow = ItemDataTable->FindRow<FItemInfoRowNew>(RowName, "");
		if (SelectedRow != nullptr)
		{
			const int32 Cost = ObjectNum * SelectedRow->Credits;
			if (Cost <= Credits || !IsGoods)
			{
				EItemTypeNew ItemType = SelectedRow->ItemTypeNew;
				const FName ItemName = SelectedRow->ItemName;
				const int32 Priority = SelectedRow->Priority;
				if (ItemType == EItemTypeNew::MedicalSupply)
				{
					// 医疗补给
					UE_LOG(LogTemp, Warning, TEXT("Buy Medical Kits!"));
					if (MedicalMap.Contains(ItemName))
					{
						MedicalMap[ItemName] += ObjectNum;
					} else
					{
						MedicalMap.Emplace(ItemName, ObjectNum);
						PriorityMap.Emplace(ItemName, Priority);
						DataRowMap.Emplace(ItemName, RowName);  // 物品对应的数据行名称
					}
				} else
				{
					// 弹药补给与武器
					UE_LOG(LogTemp, Warning, TEXT("Buy Weapon and Ammo Supply!"));
					if (WeaponMap.Contains(ItemName))
					{
						WeaponMap[ItemName] += ObjectNum;
					} else
					{
						WeaponMap.Emplace(ItemName, ObjectNum);
						PriorityMap.Emplace(ItemName, Priority);
						DataRowMap.Emplace(ItemName, RowName);  // 物品对应的数据行名称
					}
				}
				if (IsGoods)
				{
					RemoveCredits(Cost);  // 消耗积分
				}
				return true;
			}
		}
	}
	return false;
}

bool ASGPlayerState::DecreaseOrRemovalOfItem(FName ItemName, EItemTypeNew ItemType, int32 ObjectNum)
{
	if (ItemType == EItemTypeNew::MedicalSupply)
	{
		if (MedicalMap.Contains(ItemName))
		{
			MedicalMap[ItemName] -= ObjectNum;
			if (MedicalMap[ItemName] == 0)
			{
				MedicalMap.Remove(ItemName);
				PriorityMap.Remove(ItemName);
				DataRowMap.Remove(ItemName);
			}

			OnBackpackStateChanged.Broadcast(this, ItemType);
			return true;
		}
	} else
	{
		if (WeaponMap.Contains(ItemName))
		{
			WeaponMap[ItemName] -= ObjectNum;
			if (WeaponMap[ItemName] == 0)
			{
				WeaponMap.Remove(ItemName);
				PriorityMap.Remove(ItemName);
				DataRowMap.Remove(ItemName);
			}
			
			OnBackpackStateChanged.Broadcast(this, ItemType);
			return true;
		}
	}
			
	return false;
}

void ASGPlayerState::SortBackpack()
{
	// 按优先度排序
	MedicalMap.KeySort([&] (const FName& FirstName, const FName& SecondName)
	{
		return PriorityMap[FirstName] < PriorityMap[SecondName];
	});

	WeaponMap.KeySort([&] (const FName& FirstName, const FName& SecondName)
	{
		return PriorityMap[FirstName] < PriorityMap[SecondName];
	});
}

void ASGPlayerState::ApplyItemInBackpack(FName RowName, int32 ObjectNum)
{
	if (ItemDataTable)
	{
		const FItemInfoRowNew* SelectedRow = ItemDataTable->FindRow<FItemInfoRowNew>(RowName, "");
		if (SelectedRow != nullptr)
		{
			EItemTypeNew ItemType = SelectedRow->ItemTypeNew;
			const FName ItemName = SelectedRow->ItemName;
			const int32 ItemEffect = SelectedRow->ItemEffect;
			if (ItemType == EItemTypeNew::MedicalSupply)
			{
				if (USGGameplayFunctionLibrary::ApplyHealing(GetPawn(), ItemEffect * ObjectNum))
				{
					DecreaseOrRemovalOfItem(ItemName, ItemType, ObjectNum);
				}
			} else if (ItemType == EItemTypeNew::AmmoSupply)
			{
				if (USGGameplayFunctionLibrary::ApplyClip(GetPawn(), ItemName, ItemEffect * ObjectNum))
				{
					DecreaseOrRemovalOfItem(ItemName, ItemType, ObjectNum);
				}
			} else
			{
				ApplySpecialItemInBackpack(*SelectedRow, ObjectNum);
			}
		}
	}
}

void ASGPlayerState::ApplySpecialItemInBackpack(const FItemInfoRowNew& SelectedRow, int32 ObjectNum)
{
	//  使用资产管理器 - 实现Weapon资产的同步加载
	if (UAssetManager* AssetManager = UAssetManager::GetIfValid())
	{
		const FPrimaryAssetId LoadedId = SelectedRow.ItemId;

		UE_LOG(LogTemp, Log, TEXT("Begin to Load Weapon Asset......"));

		TArray<FName> Bundles;
		FStreamableDelegate Delegate = FStreamableDelegate::CreateUObject(this, &ASGPlayerState::OnWeaponAssetLoaded, SelectedRow, LoadedId, ObjectNum);
		AssetManager->LoadPrimaryAsset(LoadedId, Bundles, Delegate);
	}
}

void ASGPlayerState::OnWeaponAssetLoaded(FItemInfoRowNew SelectedRow, FPrimaryAssetId LoadedId, int32 ObjectNum)
{
	UAssetManager* AssetManager  = UAssetManager::GetIfValid();
	if (AssetManager)
	{
		USGItemDataAsset* ItemDataAsset = Cast<USGItemDataAsset>(AssetManager->GetPrimaryAssetObject(LoadedId));
		if (ItemDataAsset)
		{
			FVector SpawnStartLocation = GetPawn()->GetPawnViewLocation() + GetPawn()->GetControlRotation().Vector() * 10.0f;
			FVector SpawnEndLocation = SpawnStartLocation + GetPawn()->GetControlRotation().Vector() * 420.0f;
			GetWorld()->SpawnActor<AActor>(ItemDataAsset->ItemClass, SpawnEndLocation, FRotator::ZeroRotator);
			UE_LOG(LogTemp, Log, TEXT("Have Finished Weapon Loaded......"));
			DecreaseOrRemovalOfItem(SelectedRow.ItemName, SelectedRow.ItemTypeNew, ObjectNum);
		}
	}
}

bool ASGPlayerState::PickupItemToBackpack(FName RowName)
{
	return PurchaseAndAddToBackpack(RowName, 1, false);
}

bool ASGPlayerState::UpdatePersonalRecord(float NewTime)
{
	if (NewTime > PersonalRecordTime)
	{
		float OldRecordTime = PersonalRecordTime;
		PersonalRecordTime = NewTime;
		OnRecordTimeChanged.Broadcast(this, PersonalRecordTime, OldRecordTime);
		return true;
	}

	return false;
}

int32 ASGPlayerState::GetCredits() const
{
	return Credits;
}


