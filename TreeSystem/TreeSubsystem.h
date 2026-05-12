// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TreeActor.h"
#include "TreeSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class SURVIVAL_API UTreeSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	ATreeActor* CreateTreeActor(FTransform FT, UStaticMesh *Mesh, UFoliageInstancedStaticMeshComponent* SourceFoliage);
	void ReturnTreeToFoliage();
	
private:
	FTimerHandle TimerHandle;
	
	UPROPERTY()
	TArray<ATreeActor*> TreeActors;
};
