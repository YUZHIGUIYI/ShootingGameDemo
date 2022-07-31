// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/SGInteractionComponent.h"
#include "DrawDebugHelpers.h"
#include "Blueprint/UserWidget.h"
#include "Interface/SGGameplayInterface.h"
#include "UI/SGWorldWidget.h"

// 用于视线可视化调试
static TAutoConsoleVariable<bool> CVarDebugDrawInteraction(TEXT("su.InteractionDebugDraw"), false, TEXT("Enable Debug Lines for Interact Component."), ECVF_Cheat);

USGInteractionComponent::USGInteractionComponent():
	TraceDistance(500.0f),
	TraceRadius(30.0f),
	CollisionChannel(ECC_WorldDynamic)
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickGroup = TG_PostUpdateWork;
}

void USGInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	Player = Cast<ASGCharacterBase>(GetOwner());
}

void USGInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//APawn* MyPawn = Cast<APawn>(MyOwner);  // @Fixme later
	if (Player->IsLocallyControlled())
	{
		FindBestInteractable();
	}
}

void USGInteractionComponent::FindBestInteractable()
{
	bool bDebugDraw = CVarDebugDrawInteraction.GetValueOnGameThread();  // 仅在游戏线程下运行

	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(CollisionChannel);
	ObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn);  // ShopAssistantCharacter ???

	FVector EyeLocation;
	FRotator EyeRotation;
	Player->GetActorEyesViewPoint(EyeLocation, EyeRotation);  // 已修改为摄像机位置和旋转

	FVector End = EyeLocation + (EyeRotation.Vector() * TraceDistance);

	TArray<FHitResult> Hits;

	FCollisionShape Shape;
	Shape.SetSphere(TraceRadius);

	// 球体检测
	bool bBlockingHit = GetWorld()->SweepMultiByObjectType(Hits, EyeLocation, End, FQuat::Identity, ObjectQueryParams, Shape);

	FColor LineColor = bBlockingHit ? FColor::Green : FColor::Red;

	FocusedActor = nullptr;  // 引用置空
	for (FHitResult Hit: Hits)
	{
		if (bDebugDraw)
		{
			DrawDebugSphere(GetWorld(), Hit.ImpactPoint, TraceRadius, 32, LineColor, false, 0.0f);
		}

		AActor* HitActor = Hit.GetActor();
		if (HitActor)
		{
			if (HitActor->Implements<USGGameplayInterface>())
			{
				// 仅检测实现了该接口的Actor
				FocusedActor = HitActor;
				break;
			}
		}
	}

	// 检测到可交互Actor - 交互式UI显示 - 执行交互
	if (FocusedActor)
	{
		if (DefaultWidgetInstance == nullptr && ensure(DefaultWidgetClass))
		{
			DefaultWidgetInstance = CreateWidget<USGWorldWidget>(GetWorld(), DefaultWidgetClass);
		}

		if (DefaultWidgetInstance)
		{
			DefaultWidgetInstance->AttachedActor = FocusedActor;
			
			if (!DefaultWidgetInstance->IsInViewport())
			{
				DefaultWidgetInstance->AddToViewport();
			}
		}
	} else
	{
		if (DefaultWidgetInstance)
		{
			DefaultWidgetInstance->RemoveFromParent();  // 未检测到可交互Actor，移除UI
		}
	}

	if (bDebugDraw)
	{
		DrawDebugLine(GetWorld(), EyeLocation, End, LineColor, false, 2.0f, 0, 0.0f);
	}
}

void USGInteractionComponent::PrimaryInteract()
{
	if (FocusedActor == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "No Focus Actor to interact");
		return;
	}
	
	ISGGameplayInterface::Execute_Interact(FocusedActor, Player);
}
