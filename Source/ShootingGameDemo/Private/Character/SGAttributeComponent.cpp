// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/SGAttributeComponent.h"
#include "GameMode/SGGameModeBase.h"

// 控制台指令 - 适用于全局的伤害系数 - 仅Debug使用
static TAutoConsoleVariable<float> CVarDamageMultiplier(TEXT("su.DamageMultiplier"), 1.0f, TEXT("Global Damage Modifier for Attribute Component."),
	ECVF_Cheat);

USGAttributeComponent::USGAttributeComponent()
{
	MaxHealth = 100.0f;
	Health = MaxHealth;

	MaxStrength = 100.0f;
	Strength = MaxStrength;

	bIsSprinting = false;

	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
	PrimaryComponentTick.SetTickFunctionEnable(true);
}

void USGAttributeComponent::BeginPlay()
{
	Super::BeginPlay();

	// 激活TickComponent
	RegisterComponent();
}

USGAttributeComponent* USGAttributeComponent::GetAttributes(AActor* FromActor)
{
	if (FromActor)
	{
		return Cast<USGAttributeComponent>(FromActor->GetComponentByClass(USGAttributeComponent::StaticClass()));
	}
	return nullptr;
}

bool USGAttributeComponent::IsActorAlive(AActor* Actor)
{
	USGAttributeComponent* AttributeComp = GetAttributes(Actor);
	if (AttributeComp)
	{
		return AttributeComp->IsAlive();
	}
	return false;
}

bool USGAttributeComponent::ApplyHealthChange(AActor* InstigatorActor, float Delta)
{
	if (!GetOwner()->CanBeDamaged() && Delta < 0.0f)
	{
		return false;
	}
	if (Delta < 0.0f)
	{
		float DamageMultiplier = CVarDamageMultiplier.GetValueOnGameThread();
		Delta *= DamageMultiplier;
	}

	float OldHealth = Health;
	float NewHealth = FMath::Clamp(Health + Delta, 0.0f, MaxHealth);  // 用于治疗

	float ActualDelta = NewHealth - OldHealth;
	Health = NewHealth;
	if (ActualDelta != 0.0f)
	{
		OnHealthChanged.Broadcast(InstigatorActor, this, NewHealth, Delta);
	}
	// 玩家重生
	if (ActualDelta < 0.0f && Health == 0.0f)
	{
		ASGGameModeBase* GameMode = GetWorld()->GetAuthGameMode<ASGGameModeBase>();
		if (GameMode)
		{
			GameMode->OnActorKilled(GetOwner(), InstigatorActor);  // 玩家重生函数
		}
	}

	return ActualDelta != 0.0f;
}

bool USGAttributeComponent::ApplyStrengthChange(AActor* InstigaActor, float Delta)
{
	float OldStrength = Strength;
	float NewStrength = FMath::Clamp(Strength + Delta, 0.0f, MaxStrength);

	float ActualDelta = NewStrength - OldStrength;
	Strength = NewStrength;
	if (ActualDelta != 0.0f)
	{
		OnStrengthChanged.Broadcast(InstigaActor, this, NewStrength, Delta);
	}

	// ???

	return ActualDelta != 0.0f;
}

bool USGAttributeComponent::Kill(AActor* InstigatorActor)
{
	// 仅用于调试，kill AI
	return ApplyHealthChange(InstigatorActor, -GetMaxHealth());
}

bool USGAttributeComponent::IsAlive() const
{
	return Health > 0.0f;
}

bool USGAttributeComponent::IsMaxHealth() const
{
	return Health == MaxHealth;
}

float USGAttributeComponent::GetHealth() const
{
	return Health;
}

float USGAttributeComponent::GetMaxHealth() const
{
	return MaxHealth;
}

bool USGAttributeComponent::IsMaxStrength() const
{
	return Strength == MaxStrength;
}

float USGAttributeComponent::GetStrength() const
{
	return Strength;
}

float USGAttributeComponent::GetMaxStrength() const
{
	return MaxStrength;
}

void USGAttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                          FActorComponentTickFunction* ThisTickFunction)
{
	// 仅在非冲刺状态且且非满体力状态下恢复体力值
	if (!bIsSprinting && !IsMaxStrength())
	{
		ApplyStrengthChange(GetOwner(), 0.2f);  // ???
	} 
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}


