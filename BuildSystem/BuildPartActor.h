// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BuildPartActor.generated.h"

class UBoxComponent;

UCLASS()
class SURVIVAL_API ABuildPartActor : public AActor
{
	GENERATED_BODY()

public:
	ABuildPartActor();
	virtual void BeginPlay() override;
	
	bool IsSnapped(ABuildPartActor *BuildPartActor);
	UBoxComponent* FindSnapBox(FTransform TF);
	
	bool CanSnapActor(FTransform TF);
	void Snap(FTransform, ABuildPartActor *BuildPartActor);
	
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* MeshComp;
	
	UPROPERTY()
	TMap<UBoxComponent*, TWeakObjectPtr<ABuildPartActor>> SocketConnections; 
	
	int ClusterNum = -1;
};
