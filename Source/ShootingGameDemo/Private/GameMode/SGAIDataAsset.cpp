// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/SGAIDataAsset.h"

FPrimaryAssetId USGAIDataAsset::GetPrimaryAssetId() const
{
	return FPrimaryAssetId("AI_Characters", GetFName());
}
