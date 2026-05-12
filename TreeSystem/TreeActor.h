// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "Engine/StaticMeshActor.h"
#include "TreeActor.generated.h"

class UFoliageInstancedStaticMeshComponent;

UENUM()
enum class ETreeState : uint8
{
	Stand,
	Falling,
	Fallen
};

UCLASS()
class SURVIVAL_API ATreeActor : public AStaticMeshActor, public IInteractable
{
	GENERATED_BODY()

public:
	ATreeActor();
	virtual void OnChop(AActor* Chopper) override;
	void TreeFalling();

public:
	UPROPERTY()
	FTransform OriginalTransform;
	
	UPROPERTY()
	UFoliageInstancedStaticMeshComponent* SourceFoliage;
	
	ETreeState TreeState = ETreeState::Stand;
	
	FTimerHandle TimerHandle;
	
	int MaxHitCount = 1;

private:
	FVector FallAxis;
	FQuat StartQuat;
	float FallSpeed = 0.8;
	float CurrentAngleDeg = 0.f;
};
