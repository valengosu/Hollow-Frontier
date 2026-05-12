#include "TreeSubsystem.h"
#include "FoliageInstancedStaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"

void UTreeSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UTreeSubsystem::ReturnTreeToFoliage, 5.0f, true);   
}

ATreeActor* UTreeSubsystem::CreateTreeActor(FTransform FT, UStaticMesh *Mesh, UFoliageInstancedStaticMeshComponent* SourceFoliage)
{
	ATreeActor* Tree = GetWorld()->SpawnActor<ATreeActor>(ATreeActor::StaticClass(), FT);
	
	Tree->GetStaticMeshComponent()->SetMobility(EComponentMobility::Movable);  
	Tree->GetStaticMeshComponent()->SetStaticMesh(Mesh); 
	Tree->SourceFoliage = SourceFoliage;
	Tree->OriginalTransform = FT;
	
	TreeActors.Add(Tree);
	return Tree;
}

void UTreeSubsystem::ReturnTreeToFoliage()
{
	APawn* Char = GetWorld()->GetFirstPlayerController()->GetPawn();
	
	int Count = 0;
	for (int i = TreeActors.Num() - 1; i >= 0; i--)
	{
		if (Count++ > 10)
			break;
		
		ATreeActor *TreeActor = TreeActors[i];
		
		if (TreeActor->TreeState != ETreeState::Fallen)
			continue;
		
		float Distance = FVector::Dist(TreeActor->GetActorLocation(), Char->GetActorLocation());
		if (Distance <= 1000)
			continue;
		
		TreeActor->SourceFoliage->AddInstance(TreeActor->OriginalTransform);
		TreeActor->Destroy();
		TreeActors.RemoveAt(i);
	}
}
