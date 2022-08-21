// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SGSmartObject.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Components/ArrowComponent.h"
#include "Components/BillboardComponent.h"

ASGSmartObject::ASGSmartObject()
{
	PrimaryActorTick.bCanEverTick = false;

	BillboardComp = CreateDefaultSubobject<UBillboardComponent>("Billboard");
	RootComponent = BillboardComp;

	ArrowComp = CreateDefaultSubobject<UArrowComponent>("Arrow");
	ArrowComp->SetupAttachment(RootComponent);

}

UBehaviorTree* ASGSmartObject::GetSubtree() const
{
	return Subtree;
}

