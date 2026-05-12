// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ABuildPreviewActor.generated.h"

enum class EBuildPlacementType;
class ABuildPartActor;

UCLASS()
class SURVIVAL_API AABuildPreviewActor : public AActor
{
	GENERATED_BODY()

public:
	AABuildPreviewActor();
	void SetMaterial(UMaterialInterface* Material, int32 Index = 0);
	
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* MeshComp;
	
	UPROPERTY(EditAnywhere, Category="Build")
	UMaterialInterface* GreeMat;
	
	UPROPERTY(EditAnywhere, Category="Build")
	UMaterialInterface* RedMat;
	
	TSubclassOf<ABuildPartActor> BuildPartActorClass;
	TEnumAsByte<ETraceTypeQuery> SocketChannel;
	
	UPROPERTY(EditAnywhere)
	ABuildPartActor* SnapActor;
	
	UPROPERTY(EditAnywhere)
	UPrimitiveComponent* SnapComponent;
};
