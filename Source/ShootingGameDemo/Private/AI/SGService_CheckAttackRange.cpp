// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SGService_CheckAttackRange.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

USGService_CheckAttackRange::USGService_CheckAttackRange()
{
	TargetActorKey = "TargetActor";
	MaxAttackRange = 1800.0f;
}

void USGService_CheckAttackRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	// 检查AI与Player之间的距离
	if (ensure(BlackboardComp))
	{
		AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject(TargetActorKey));
		if (TargetActor)
		{
			AAIController* AIController = OwnerComp.GetAIOwner();
			APawn* AIPawn = AIController->GetPawn();
			if (ensure(AIPawn))
			{
				// 计算AI与Player之间的距离
				float DistanceBetween = FVector::Distance(TargetActor->GetActorLocation(), AIPawn->GetActorLocation());
				// 是否在最大攻击范围内
				bool bWithinAttackRange = DistanceBetween < MaxAttackRange;

				/*bool bHasLOS = false;
				if (bWithinAttackRange)
				{
					bHasLOS = AIController->LineOfSightTo(TargetActor);
				}*/
				bool bHasLOS = false;
				if (bWithinAttackRange)
				{
					bHasLOS = BlackboardComp->GetValueAsBool(FoundTargetKey.SelectedKeyName);
				}

				BlackboardComp->SetValueAsBool(AttackRangeKey.SelectedKeyName, (bWithinAttackRange && bHasLOS));  // ??? @check
			}
		}
	}
}
