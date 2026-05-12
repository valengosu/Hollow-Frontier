// Fill out your copyright notice in the Description page of Project Settings.


#include "ABuildPreviewActor.h"


// Sets default values
AABuildPreviewActor::AABuildPreviewActor()
{
	PrimaryActorTick.bCanEverTick = false;
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	RootComponent = MeshComp;
	
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AABuildPreviewActor::SetMaterial(UMaterialInterface* Material, int32 Index)
{
	MeshComp->SetMaterial(Index, Material);
}


