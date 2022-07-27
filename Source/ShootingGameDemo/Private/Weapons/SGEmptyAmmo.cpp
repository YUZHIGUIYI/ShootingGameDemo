// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/SGEmptyAmmo.h"
#include "Components/ArrowComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

ASGEmptyAmmo::ASGEmptyAmmo()
{
	PrimaryActorTick.bCanEverTick = true;
	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComp");
	RootComponent = StaticMeshComp;

	ArrowComp = CreateDefaultSubobject<UArrowComponent>("ArrowComp");
	ArrowComp->SetupAttachment(RootComponent);

	StaticMeshComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	StaticMeshComp->SetSimulatePhysics(true);
	StaticMeshComp->SetNotifyRigidBodyCollision(true);
	StaticMeshComp->BodyInstance.SetInstanceNotifyRBCollision(true);

	StaticMeshComp->OnComponentHit.AddDynamic(this, &ASGEmptyAmmo::OnComponentHitHappened);

	MassInKg = 0.2f;
	ImpulseParm = 50.0f;
	
}

void ASGEmptyAmmo::BeginPlay()
{
	Super::BeginPlay();

	FVector Impulse = ArrowComp->GetForwardVector() * ImpulseParm;
	FVector Location = ArrowComp->GetComponentLocation();
	StaticMeshComp->AddImpulseAtLocation(Impulse, Location);
	SetLifeSpan(3.0f);
}

void ASGEmptyAmmo::OnComponentHitHappened(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (Hit.bBlockingHit)
	{
		FVector ComponentVelocity = HitComponent->GetComponentVelocity();
		float ComponentVelocityLength = ComponentVelocity.Size();
		if (ComponentVelocityLength > 100.0f)
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), LangdingSound, Hit.ImpactPoint, FRotator::ZeroRotator);
		}
	}
}

void ASGEmptyAmmo::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

