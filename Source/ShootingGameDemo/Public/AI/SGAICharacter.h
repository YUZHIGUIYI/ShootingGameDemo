// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/SGDamageInterface.h"
#include "Perception/AIPerceptionTypes.h"
#include "SGAICharacter.generated.h"

class USGAttributeComponent;  // 属性组件
class UUserWidget;
class USGWorldWidget;         // 生命条
class USGActionComponent;     // 动作组件
class UAIPerceptionComponent;  // 感知源组件
class UAISenseConfig_Sight;  // 配置文件
class USGAction;  

UCLASS()
class SHOOTINGGAMEDEMO_API ASGAICharacter : public ACharacter, public ISGDamageInterface
{
	GENERATED_BODY()

public:
	
	ASGAICharacter();

protected:

	UPROPERTY()
	USGWorldWidget* AIHealthBar;  // 需要声明为UPROPERTY吗？？？

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> HealthBarWidgetClass;

	// AI发现玩家时显示提示UI
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> SpottedWidgetClass;

	// 受击特效？

	// 燃烧Effect - 火箭弹或榴弹的额外伤害
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ActionEffect")
	TSubclassOf<USGAction> BurningEffectClass;

	// 对应于AI行为树的Blackboard的TargetActor
	UPROPERTY(VisibleAnywhere, Category = "Blackboard")
	FName TargetActorKey;

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UAIPerceptionComponent* AIPerceptionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UAISenseConfig_Sight* AIConfigSight;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USGAttributeComponent* AttributeComp;  // 属性组件

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USGActionComponent* ActionComp;  // 动作组件
	
protected:

	UFUNCTION(BlueprintCallable, Category = "AI")
	void SetTargetActor(AActor* NewTarget);

	UFUNCTION(BlueprintCallable, Category = "AI")
	AActor* GetTargetActor() const;

	// AI受击时特效
	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, USGAttributeComponent* OwningComp, float NewHealth, float Delta);

	UFUNCTION()
	void OnTargetUpdated(AActor* Actor, FAIStimulus Stimulus);
	
	virtual void PostInitializeComponents() override;

protected:
	
	virtual void BeginPlay() override;

public:

	// 接收普通伤害
	void TakeDirectDamage_Implementation(APawn* InstigatorPawn, const FHitResult& ImpactResult, float Damage) override;

	// 接收爆炸伤害
	void TakeExplosiveDamage_Implementation(APawn* InstigatorPawn, const FHitResult& ImpactResult, float Damage) override;

};
