// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/SGSmartObject.h"
#include "SGAIPatrolRoute.generated.h"

class USplineComponent;

UCLASS()
class SHOOTINGGAMEDEMO_API ASGAIPatrolRoute : public ASGSmartObject
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere)
	USplineComponent* SplineComp;

	TArray<FVector> RoutePoints;

protected:

	void BeginPlay() override;
	
public:

	ASGAIPatrolRoute();

	TArray<FVector>& GetRoutePoints();
};
