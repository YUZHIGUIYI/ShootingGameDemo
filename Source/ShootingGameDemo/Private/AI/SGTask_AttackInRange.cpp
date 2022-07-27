// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SGTask_AttackInRange.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/SGAttributeComponent.h"
#include "GameFramework/Character.h"

USGTask_AttackInRange::USGTask_AttackInRange()
{
	TargetActorKey = "TargetActor";
	MuzzleSocketName = "Muzzle_01";
	MaxAmmoOffset = 3.0f;
}

EBTNodeResult::Type USGTask_AttackInRange::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (ensure(AIController))
	{
		ACharacter* AICharacter = Cast<ACharacter>(AIController->GetPawn());
		if (AICharacter == nullptr)
		{
			return EBTNodeResult::Failed;  // 执行失败
		}

		AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TargetActorKey));
		if (TargetActor == nullptr)
		{
			return EBTNodeResult::Failed;
		}
		// Player是否死亡
		if (!USGAttributeComponent::IsActorAlive(TargetActor))
		{
			return EBTNodeResult::Failed;
		}
		// AI是否死亡
		if (!USGAttributeComponent::IsActorAlive(AICharacter))
		{
			return EBTNodeResult::Failed;
		}

		FVector MuzzleLocation = AICharacter->GetMesh()->GetSocketLocation(MuzzleSocketName);
		FVector Direction = TargetActor->GetActorLocation() - MuzzleLocation;
		FRotator MuzzleRotation = Direction.Rotation();

		MuzzleRotation.Pitch += FMath::RandRange(0.0f, MaxAmmoOffset);
		MuzzleRotation.Yaw += FMath::RandRange(-MaxAmmoOffset, MaxAmmoOffset);

		FActorSpawnParameters Params;
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		Params.Instigator = AICharacter;

		AActor* SpawnAmmo = GetWorld()->SpawnActor<AActor>(AmmoClass, MuzzleLocation, MuzzleRotation, Params);

		return SpawnAmmo ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
	}
	return EBTNodeResult::Failed;
}

