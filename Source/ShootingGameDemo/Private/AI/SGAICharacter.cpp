// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SGAICharacter.h"
#include "AIController.h"
#include "BrainComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/SGActionComponent.h"
#include "Character/SGAttributeComponent.h"
#include "Components/CapsuleComponent.h"
#include "FunctionLibrary/SGGameplayFunctionLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "UI/SGWorldWidget.h"

ASGAICharacter::ASGAICharacter()
{
	AttributeComp = CreateDefaultSubobject<USGAttributeComponent>("AttributeComp");

	ActionComp = CreateDefaultSubobject<USGActionComponent>("ActionComp");

	AIPerceptionComp = CreateDefaultSubobject<UAIPerceptionComponent>("AIPerceptionComp");
	AIConfigSight = CreateDefaultSubobject<UAISenseConfig_Sight>("AIConfigSight");

	AIConfigSight->SightRadius = 4000.0f;
	AIConfigSight->LoseSightRadius = 4500.0f;
	AIConfigSight->PeripheralVisionAngleDegrees = 60.0f;
	AIConfigSight->DetectionByAffiliation.bDetectNeutrals = true;
	AIConfigSight->DetectionByAffiliation.bDetectEnemies = true;
	AIConfigSight->DetectionByAffiliation.bDetectFriendlies = true;

	AIPerceptionComp->ConfigureSense(*AIConfigSight);
	AIPerceptionComp->SetDominantSense(UAISenseConfig_Sight::StaticClass());

	// 确保AI控制器可以控制在世界中生成的AI
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	// 允许产生干涉事件 - 碰撞设置
	GetMesh()->SetGenerateOverlapEvents(true);

	TargetActorKey = "TargetActor";
}

void ASGAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	// 绑定事件
	//PawnSensingComp->OnSeePawn.AddDynamic(this, &ASGAICharacter::OnPawnFound);
	AIPerceptionComp->OnTargetPerceptionUpdated.AddDynamic(this, &ASGAICharacter::OnTargetUpdated);
	AttributeComp->OnHealthChanged.AddDynamic(this, &ASGAICharacter::OnHealthChanged);
}

void ASGAICharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ASGAICharacter::TakeDirectDamage_Implementation(APawn* InstigatorPawn, const FHitResult& ImpactResult, float Damage)
{
	if (USGGameplayFunctionLibrary::CheckGroupByGameplayTag(InstigatorPawn, this))
	{
		return;  // 检查两者的第一个GameplayTag是否相同，相同则不造成伤害
	}
	
	float DamageThresholdScale = 1.0f;
	const UPhysicalMaterial* PhysicalMaterial = ImpactResult.PhysMaterial.Get();
	if (PhysicalMaterial != nullptr)
	{
		DamageThresholdScale = PhysicalMaterial->DestructibleDamageThresholdScale;
		UE_LOG(LogTemp, Log, TEXT("Direct Hit Physical Material is: %s"), *PhysicalMaterial->GetName());
	}
	
	AttributeComp->ApplyHealthChange(InstigatorPawn, -Damage * DamageThresholdScale);
	//EPhysicalSurface SurfaceType = UPhysicalMaterial::DetermineSurfaceType(ImpactResult.PhysMaterial.Get());
	/*switch (SurfaceType)
	{
	case EPhysicalSurface::SurfaceType_Default:
		break;
	case EPhysicalSurface::SurfaceType1:
		break;
	default:
		break;
	}*/
}

void ASGAICharacter::TakeExplosiveDamage_Implementation(APawn* InstigatorPawn, const FHitResult& ImpactResult, float Damage)
{
	float DamageThresholdScale = 1.0f;
	const UPhysicalMaterial* PhysicalMaterial = ImpactResult.PhysMaterial.Get();
	if (PhysicalMaterial != nullptr)
	{
		DamageThresholdScale = PhysicalMaterial->DestructibleDamageThresholdScale;
		UE_LOG(LogTemp, Log, TEXT("Explode Hit Physical Material is: %s"), *PhysicalMaterial->GetName());
	}
	AttributeComp->ApplyHealthChange(InstigatorPawn, -Damage * DamageThresholdScale);
	// 附加燃烧伤害 - 施加方为InstigatorPawn, 受作用方为AICharacter
	ActionComp->AddAction(InstigatorPawn, BurningEffectClass);
}

void ASGAICharacter::SetTargetActor(AActor* NewTarget)
{
	// 获取AI控制器
	AAIController* AIController = Cast<AAIController>(GetController());
	// 为AI行为树的Blackboard Object赋值
	if (AIController)
	{
		AIController->GetBlackboardComponent()->SetValueAsObject(TargetActorKey, NewTarget);
	}
}

AActor* ASGAICharacter::GetTargetActor() const
{
	AAIController* AIController = Cast<AAIController>(GetController());
	if (AIController)
	{
		return Cast<AActor>(AIController->GetBlackboardComponent()->GetValueAsObject(TargetActorKey));
	}

	return nullptr;
}

void ASGAICharacter::OnHealthChanged(AActor* InstigatorActor, USGAttributeComponent* OwningComp, float NewHealth,
	float Delta)
{
	// AI受到 Player 攻击时，将 Blackboard 中的 TargetActor 设置为攻击者
	// 同时显示当前 AIHealthBar 血条
	if (Delta < 0.0f)
	{
		if (InstigatorActor != this)
		{
			SetTargetActor(InstigatorActor);
		}

		if (AIHealthBar == nullptr)
		{
			AIHealthBar = CreateWidget<USGWorldWidget>(GetWorld(), HealthBarWidgetClass);
			if (AIHealthBar)
			{
				AIHealthBar->AttachedActor = this;
				AIHealthBar->AddToViewport(10);  // ?? 
			}
		}

		//  特效

		// AI死亡时
		if (NewHealth <= 0.0f)
		{
			// 停止AI行为树
			AAIController* AIController = Cast<AAIController>(GetController());
			if (AIController)
			{
				AIController->GetBrainComponent()->StopLogic("AI Has Been Killed!");
			}

			// 布娃娃
			GetMesh()->SetAllBodiesSimulatePhysics(true);  // 开启模拟物理
			GetMesh()->SetCollisionProfileName("Ragdoll");

			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);  // 设置胶囊体无碰撞
			GetCharacterMovement()->DisableMovement();  // 停止移动组件

			SetLifeSpan(5.0f);  // 设置生命周期
		}
	}
}

void ASGAICharacter::OnTargetUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	AAIController* AIController = Cast<AAIController>(GetController());
	if (Stimulus.IsValid())
	{
		if (GetTargetActor() != Actor)
		{
			SetTargetActor(Actor);
			// 单机状态下
			USGWorldWidget* SpottedWidget = CreateWidget<USGWorldWidget>(GetWorld(), SpottedWidgetClass);
			if (SpottedWidget)
			{
				SpottedWidget->AttachedActor = this;
				SpottedWidget->AddToViewport(20);  // 确保该UI显示在任何UI的前面
			}
		}
		AIController->GetBlackboardComponent()->SetValueAsBool("bFoundTarget", true);
	} else
	{
		AIController->GetBlackboardComponent()->SetValueAsBool("bFoundTarget", false);
	}
}

