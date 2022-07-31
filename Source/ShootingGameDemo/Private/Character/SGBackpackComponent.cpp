// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/SGBackpackComponent.h"
#include "Engine/AssetManager.h"
#include "FunctionLibrary/SGGameplayFunctionLibrary.h"
#include "GameMode/SGItemDataAsset.h"

USGBackpackComponent::USGBackpackComponent()
{
}

void USGBackpackComponent::CheckCreditsAndPurchase(FName ObjectName, FName RowName, EItemType ItemType, int32 ObjectNum,
	int32 Priority)
{
	if (ItemDataTable)
	{
		const FItemInfoRow* SelectedRow = ItemDataTable->FindRow<FItemInfoRow>(RowName, "");
		
		if (SelectedRow == nullptr) return;
		
		const int32 Cost = SelectedRow->Credits;
		if (USGGameplayFunctionLibrary::CheckCreditsWhenPurchasing(GetOwner(), Cost))
		{
			AddIntoBackpack(ObjectName, RowName, ItemType, ObjectNum, Priority);
		}
	}
}

void USGBackpackComponent::AddIntoBackpack(FName ObjectName, FName RowName, EItemType ItemType, int32 ObjectNum, int32 Priority)
{
	if (ItemType == EItemType::MedicalKit)
	{
		UE_LOG(LogTemp, Warning, TEXT("Buy Medical Kits!"));
		if (BackpackMedicalMap.Contains(ObjectName))
		{
			BackpackMedicalMap[ObjectName].IArray[0] += ObjectNum;
		} else
		{
			FItemArray FIArray;
			FIArray.IArray[0] = ObjectNum;
			FIArray.IArray[1] = Priority;
			BackpackMedicalMap.Emplace(ObjectName, FIArray);
			NameToRowMap.Emplace(ObjectName, RowName);
		}
	} else
	{
		UE_LOG(LogTemp, Warning, TEXT("Buy Weapon!"));
		if (BackpackWeaponMap.Contains(ObjectName))
		{
			BackpackWeaponMap[ObjectName].IArray[0] += ObjectNum;
		} else
		{
			FItemArray FIArray;
			FIArray.IArray[0] = ObjectNum;
			FIArray.IArray[1] = Priority;
			BackpackWeaponMap.Emplace(ObjectName, FIArray);
			NameToRowMap.Emplace(ObjectName, RowName);
		}
	}
}

void USGBackpackComponent::SortBackpack()
{
	BackpackMedicalMap.ValueSort([] (const FItemArray& FirstItem, const FItemArray& SecondItem)
	{
		return FirstItem.IArray[1] < SecondItem.IArray[1];  // 根据优先度升序排序
	});

	BackpackWeaponMap.ValueSort([] (const FItemArray& FirstItem, const FItemArray& SecondItem)
	{
		return FirstItem.IArray[1] < SecondItem.IArray[1];  // 根据优先度升序排序
	});
}

bool USGBackpackComponent::RemoveFromBackpack(FName ObjectName)
{
	int32 NumRemoved = BackpackMedicalMap.Remove(ObjectName);
	if (NumRemoved == 0)
	{
		return false;
	}
	return true;
}

void USGBackpackComponent::DecreaseOfItemNumber(FName ObjectName, EItemType ItemType)
{
	if (ItemType == EItemType::MedicalKit)
	{
		if (BackpackMedicalMap.Contains(ObjectName))
		{
			BackpackMedicalMap[ObjectName].IArray[0] -= 1;
			if (BackpackMedicalMap[ObjectName].IArray[0] == 0)
			{
				BackpackMedicalMap.Remove(ObjectName);
				NameToRowMap.Remove(ObjectName);
			}
		}
	} else
	{
		if (BackpackWeaponMap.Contains(ObjectName))
		{
			BackpackWeaponMap[ObjectName].IArray[0] -= 1;
			if (BackpackWeaponMap[ObjectName].IArray[0] == 0)
			{
				BackpackWeaponMap.Remove(ObjectName);
				NameToRowMap.Remove(ObjectName);
			}
		}
	}

	OnBackpackChanged.Broadcast(this, ItemType);
}

void USGBackpackComponent::ApplyGeneralItem(FName ObjectName, FName RowName, EItemType ItemType)
{
	if (ItemType == EItemType::MedicalKit)
	{
		if (ItemDataTable)
		{
			const FItemInfoRow* SelectedRow = ItemDataTable->FindRow<FItemInfoRow>(RowName, "");
			if (SelectedRow == nullptr) return;
			if (USGGameplayFunctionLibrary::ApplyHealing(GetOwner(), SelectedRow->ItemEffect))
			{
				DecreaseOfItemNumber(ObjectName, ItemType);
			}
		}
	} else
	{
		if (ItemDataTable)
		{
			const FItemInfoRow* SelectedRow = ItemDataTable->FindRow<FItemInfoRow>(RowName, "");
			if (SelectedRow == nullptr) return;
			if (SelectedRow->Priority > 8)
			{
				UE_LOG(LogTemp, Log, TEXT("Apply Special Item......"));
				ApplySpecialItem(*SelectedRow, ObjectName, ItemType);
			} else if (USGGameplayFunctionLibrary::ApplyClip(GetOwner(), ObjectName, SelectedRow->ItemEffect))
			{
				DecreaseOfItemNumber(ObjectName, ItemType);
			}
		}
	}
}

void USGBackpackComponent::ApplySpecialItem(const FItemInfoRow& SelectedRow, FName ObjectName, EItemType ItemType)
{
	// 使用资产管理器 - 启动SpecialItem同步加载
	if (UAssetManager* AssetManager = UAssetManager::GetIfValid())
	{
		const FPrimaryAssetId SpecialItemId = SelectedRow.ItemId;

		UE_LOG(LogTemp, Log, TEXT("Begin to Load Special Item......"));

		TArray<FName> Bundles;
		FStreamableDelegate Delegate = FStreamableDelegate::CreateUObject(this, &USGBackpackComponent::OnSpecialItemLoaded,
			SpecialItemId, ObjectName, ItemType);
		AssetManager->LoadPrimaryAsset(SpecialItemId, Bundles, Delegate);
	}
}

void USGBackpackComponent::OnSpecialItemLoaded(FPrimaryAssetId LoadedId, FName ObjectName, EItemType ItemType)
{
	UAssetManager* AssetManager = UAssetManager::GetIfValid();
	if (AssetManager)
	{
		USGItemDataAsset* ItemDataAsset = Cast<USGItemDataAsset>(AssetManager->GetPrimaryAssetObject(LoadedId));
		if (ItemDataAsset)
		{
			FVector SpawnLocation = GetOwner()->GetActorLocation();
			SpawnLocation.X += 100.0f;
			GetWorld()->SpawnActor<AActor>(ItemDataAsset->ItemClass, SpawnLocation, FRotator::ZeroRotator);
			UE_LOG(LogTemp, Log, TEXT("Have Finished Special Item Loading......"));

			DecreaseOfItemNumber(ObjectName, ItemType);
		}
	}
}


