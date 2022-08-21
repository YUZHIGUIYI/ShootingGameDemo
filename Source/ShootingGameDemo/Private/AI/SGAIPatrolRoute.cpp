// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SGAIPatrolRoute.h"

#include "Components/SplineComponent.h"



ASGAIPatrolRoute::ASGAIPatrolRoute()
{
	SplineComp = CreateDefaultSubobject<USplineComponent>("PatrolRoute");
	SplineComp->SetupAttachment(GetRootComponent());
}
void ASGAIPatrolRoute::BeginPlay()
{
	Super::BeginPlay();

	// 将路径点对应的世界坐标添加进数组
	for (int i = 0; i < SplineComp->GetNumberOfSplinePoints(); ++i)
	{
		RoutePoints.AddUnique(SplineComp->GetWorldLocationAtSplinePoint(i));
	}
}
TArray<FVector>& ASGAIPatrolRoute::GetRoutePoints()
{
	return RoutePoints;
}
