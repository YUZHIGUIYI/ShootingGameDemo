// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/SGItemDataAsset.h"

FPrimaryAssetId USGItemDataAsset::GetPrimaryAssetId() const
{
	return FPrimaryAssetId("SpecificItems", GetFName());
}
