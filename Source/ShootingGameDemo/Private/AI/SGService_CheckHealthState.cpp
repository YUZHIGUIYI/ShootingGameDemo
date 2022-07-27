// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SGService_CheckHealthState.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/SGAttributeComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

USGService_CheckHealthState::USGService_CheckHealthState()
{
	Speed = 900.0f;
	HealthThreshold = 0.35f;
}

void USGService_CheckHealthState::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* AIPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (ensure(AIPawn))
	{
		USGAttributeComponent* AttributeComp = USGAttributeComponent::GetAttributes(AIPawn);
		if (ensure(AttributeComp))
		{
			bool bHealthThreshold = (AttributeComp->GetHealth() / AttributeComp->GetMaxHealth()) < HealthThreshold;
			
			UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
			BlackboardComp->SetValueAsBool(HealthThresholdKey.SelectedKeyName, bHealthThreshold);

			ACharacter* AICharacter = Cast<ACharacter>(AIPawn);
			if (bHealthThreshold)
			{
				AICharacter->GetCharacterMovement()->MaxWalkSpeed = Speed;
			} else
			{
				AICharacter->GetCharacterMovement()->MaxWalkSpeed = 600.0f;
			}
		}
	}
}
