#include "TreeActor.h"

#include "Character/SurvivalCharacter.h"
#include "SurvivalComponent/InventoryComponent.h"
#include "UISystem/UIManagerSubsystem.h"

ATreeActor::ATreeActor()
{
	PrimaryActorTick.bCanEverTick = false;
	
	UStaticMeshComponent* Mesh = GetStaticMeshComponent();
	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Mesh->SetMobility(EComponentMobility::Movable);
	Mesh->SetGenerateOverlapEvents(true);
}

void ATreeActor::OnChop(AActor* Chopper)
{
	if (TreeState != ETreeState::Stand)
		return;
	
	if (MaxHitCount-- == 0)
	{
		if (ASurvivalCharacter* Char = Cast<ASurvivalCharacter>(Chopper))
		{
			UUIManagerSubsystem* UIManagerSubsystem = Char->GetController<APlayerController>()->GetLocalPlayer()->GetSubsystem<UUIManagerSubsystem>();
			UIManagerSubsystem->AddItem("Wood",20);
		}
		
		TreeState = ETreeState::Falling;
		GetStaticMeshComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		
		FVector Dir = GetActorLocation() - Chopper->GetActorLocation();
		Dir.Z = 0.f;
		Dir.Normalize();
		
		FallAxis = FVector::CrossProduct(FVector::UpVector, Dir);
		FallAxis.Normalize();
		
		StartQuat = GetActorQuat();
		CurrentAngleDeg = 0;
		
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ATreeActor::TreeFalling, 0.02f, true);
	}
}

void ATreeActor::TreeFalling()
{
	CurrentAngleDeg += FallSpeed;
	float AngleRad = FMath::DegreesToRadians(CurrentAngleDeg);
	FQuat DeltaQuat(FallAxis, AngleRad);
	SetActorRotation((DeltaQuat * StartQuat).Rotator());
	
	if (CurrentAngleDeg >= 90.f)
	{
		TreeState = ETreeState::Fallen;
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	}
}