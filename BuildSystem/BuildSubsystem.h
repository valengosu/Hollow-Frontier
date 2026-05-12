// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/LocalPlayerSubsystem.h"
#include "../SurvivalComponent/InteractionComponent.h"
#include "BuildSubsystem.generated.h"

class ABuildPartActor;
class AABuildPreviewActor;
/**
 * 
 */

UCLASS()
class SURVIVAL_API UBuildSubsystem : public ULocalPlayerSubsystem
{
	GENERATED_BODY()
	
public:
	void PreparePreviewActor();
	void PlacePreviewActor();
	
	void PlaceBuildPartActor();
	
	bool TryPlaceBuildPart(AABuildPreviewActor* Actor);
	bool CanPlaceBuildPart(AABuildPreviewActor* Actor, TArray<ABuildPartActor*>* SnapBuildPartActors = nullptr);
	
	void ActiveBuildMode(bool bo, FName ItemName = NAME_None);
	//void Demolish();
	
	UPROPERTY()
	AABuildPreviewActor* PreviewActor;
	
	UPROPERTY()
	FTimerHandle PreviewTimerHandle;
	
	static int ClusterNum;
	TMap<int, TSet<TWeakObjectPtr<ABuildPartActor>>> BuildingClusters;
};
