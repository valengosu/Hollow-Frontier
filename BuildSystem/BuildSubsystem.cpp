#include "BuildSubsystem.h"
#include "ABuildPreviewActor.h"
#include "BuildPartActor.h"
#include "Engine/World.h"
#include "CollisionQueryParams.h"
#include "Engine/OverlapResult.h"
#include "WorldCollision.h"
#include "Landscape.h"
#include "Character/SurvivalCharacter.h"
#include "Config/BuildPartConfig.h"
#include "Config/SurvivalGI.h"
#include "Kismet/GameplayStatics.h"
#include "UISystem/UIManagerSubsystem.h"

int UBuildSubsystem::ClusterNum = 0;

void UBuildSubsystem::PreparePreviewActor()
{
	if (PreviewActor == nullptr)
	{
		USurvivalGI* GI = Cast<USurvivalGI>(GetWorld()->GetGameInstance());
		PreviewActor = GetWorld()->SpawnActor<AABuildPreviewActor>(GI->BuildPreviewActorClass);
	}
}

bool UBuildSubsystem::TryPlaceBuildPart(AABuildPreviewActor* BuildPreviewActor)
{
	TArray<ABuildPartActor*> SnapBuildPartActors;
	
	if (CanPlaceBuildPart(BuildPreviewActor, &SnapBuildPartActors) == false)
		return false;
	
	ABuildPartActor *PartActor = GetWorld()->SpawnActor<ABuildPartActor>(BuildPreviewActor->BuildPartActorClass);
	PartActor->SetActorTransform(PreviewActor->GetActorTransform());
	
	//If the cluster numbers of the SnapBuildPartActors are different, a merge process is required. For now, this case is ignored.
	for (auto& Elem : SnapBuildPartActors)
	{
		Elem->Snap(PartActor->GetActorTransform(), PartActor);
		PartActor->Snap(Elem->GetActorTransform(), Elem);
	}
	
	int CurrentClusterNum = SnapBuildPartActors.Num() == 0 ? ClusterNum++ : SnapBuildPartActors[0]->ClusterNum;
	PartActor->ClusterNum = CurrentClusterNum;
	
	//UE_LOG(LogTemp, Warning, TEXT("Hello %d"), PartActor->ClusterNum);
	BuildingClusters.FindOrAdd(CurrentClusterNum).Add(PartActor);
	
	return true;
}

bool UBuildSubsystem::CanPlaceBuildPart(AABuildPreviewActor* BuildPreviewActor, TArray<ABuildPartActor*>* SnapBuildPartActors)
{
	if (BuildPreviewActor == nullptr) 
		return false;
	
	FVector Extent = BuildPreviewActor->MeshComp->GetStaticMesh()->GetBounds().BoxExtent * BuildPreviewActor->MeshComp->GetComponentScale();
	FCollisionShape Shape = FCollisionShape::MakeBox(Extent);
	
	TArray<FOverlapResult> Overlaps;
	GetWorld()->OverlapMultiByChannel(Overlaps, BuildPreviewActor->GetActorLocation(), BuildPreviewActor->GetActorQuat(),ECC_Visibility, Shape);
	
	if (Overlaps.Num() == 0)
		return false;
	
	for (const FOverlapResult& Result : Overlaps)
	{
		AActor* OtherActor = Result.GetActor();
		//UE_LOG(LogTemp, Warning, TEXT("Actor Name: %s"), *Actor->GetName());
		
		if (OtherActor->IsA(ALandscape::StaticClass()))
			continue;
		
		ABuildPartActor* OverlapBuildPartActor = Cast<ABuildPartActor>(OtherActor);
		if (OverlapBuildPartActor == nullptr)
			return false;
		
		//Find if Snap mode.
		UPrimitiveComponent* SnapComponent = BuildPreviewActor->SnapComponent;
		if (SnapComponent == nullptr)
			return false;
		
		//if Overlap BuildPartActor and Snap Owner are in the SAME Building Cluster then pass
		if (OverlapBuildPartActor->FindSnapBox(SnapComponent->GetComponentTransform()) == nullptr && 
			BuildingClusters[BuildPreviewActor->SnapActor->ClusterNum].Contains(OverlapBuildPartActor) == true)
			continue;
		
		if (OverlapBuildPartActor->CanSnapActor(SnapComponent->GetComponentTransform()) == false)
			return false;
		
		if (SnapBuildPartActors != nullptr)
		{
			SnapBuildPartActors->Add(OverlapBuildPartActor);
		}
	}
	
	return true;
}

void UBuildSubsystem::PlacePreviewActor()
{
	PreparePreviewActor();
	
	ASurvivalCharacter* Char = Cast<ASurvivalCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	EBuildPlacementType PlacementType = Char->InteractionComp->PreviewBuildPosition(PreviewActor);
	
	if (PlacementType == EBuildPlacementType::Invalid || CanPlaceBuildPart(PreviewActor) == false)
	{
		PreviewActor->SetMaterial(PreviewActor->RedMat);
		return;
	}
	
	PreviewActor->SetMaterial(PreviewActor->GreeMat);
}

void UBuildSubsystem::ActiveBuildMode(bool bo, FName ItemName)
{
	PreparePreviewActor();
	PreviewActor->SetActorHiddenInGame(!bo);
	
	if (bo == true)
	{
		FBuildPartConfig *BuildPartConfig = GetTableRow<FBuildPartConfig>(this, ItemName);
		if (BuildPartConfig == nullptr)
			return;
		
		PreviewActor->MeshComp->SetStaticMesh(BuildPartConfig->Mesh);
		PreviewActor->SocketChannel = BuildPartConfig->SocketChannel;
		PreviewActor->BuildPartActorClass = BuildPartConfig->BuildPartActorClass;
		
		PlacePreviewActor();
		GetWorld()->GetTimerManager().SetTimer(PreviewTimerHandle, this, &UBuildSubsystem::PlacePreviewActor, 0.05f, true);
		
		UUIManagerSubsystem* UIManager = GetLocalPlayer()->GetSubsystem<UUIManagerSubsystem>();
		UIManager->SurvivalHUDWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
	
	else if (bo == false)
	{
		GetWorld()->GetTimerManager().ClearTimer(PreviewTimerHandle);
	}
}

/*
void UBuildSubsystem::Demolish()
{
	ASurvivalCharacter* Char = Cast<ASurvivalCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	APlayerController* PC = Char->GetController<APlayerController>();
	
	FHitResult Hit;
	bool bo = PC->GetHitResultUnderCursor(ECC_Visibility, false, Hit);
	if (bo == false)
		return;
	
	ABuildPartActor* BuildPartActor = Cast<ABuildPartActor>(Hit.GetActor());
	if (BuildPartActor == nullptr)
		return;
	
	if (FVector::Dist(Char->GetActorLocation(), Hit.ImpactPoint) >= 1200.f)
		return;
	
	BuildPartActor->Destroy();
}
*/

void UBuildSubsystem::PlaceBuildPartActor()
{
	if (PreviewActor == nullptr || 
		PreviewActor->IsHidden() == true || 
		PreviewActor->MeshComp->GetMaterial(0) == PreviewActor->RedMat)
		return;
	
	if (TryPlaceBuildPart(PreviewActor) == false)
		return;
	
	ActiveBuildMode(false);
	
	UUIManagerSubsystem* UIManager = GetLocalPlayer()->GetSubsystem<UUIManagerSubsystem>();
	UIManager->SurvivalHUDWidget->SetVisibility(ESlateVisibility::Visible);
}

