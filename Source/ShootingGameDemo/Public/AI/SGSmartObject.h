// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SGSmartObject.generated.h"

class UArrowComponent;
class UBehaviorTree;

UCLASS()
class SHOOTINGGAMEDEMO_API ASGSmartObject : public AActor
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere)
	UBillboardComponent* BillboardComp;

	UPROPERTY(EditAnywhere)
	UArrowComponent* ArrowComp;

	UPROPERTY(EditAnywhere)
	UBehaviorTree* Subtree;  // 次级树 - 可以用来动态替换AI身上的行为
	
public:	
	
	ASGSmartObject();

	UBehaviorTree* GetSubtree() const;
};
