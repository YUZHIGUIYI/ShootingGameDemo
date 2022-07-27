// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SGTask_RestoreHealth.h"
#include "AIController.h"
#include "Character/SGAttributeComponent.h"

EBTNodeResult::Type USGTask_RestoreHealth::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	APawn* AIPawn = Cast<APawn>(OwnerComp.GetAIOwner()->GetPawn());
	if (AIPawn == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	USGAttributeComponent* AttributeComp = USGAttributeComponent::GetAttributes(AIPawn);
	if (AttributeComp == nullptr)
	{
		return EBTNodeResult::Failed;
	}
	
	AttributeComp->ApplyHealthChange(AIPawn, AttributeComp->GetMaxHealth());
	return EBTNodeResult::Succeeded;
}
