#include "BuildPartActor.h"
#include "Components/BoxComponent.h"

ABuildPartActor::ABuildPartActor()
{
	PrimaryActorTick.bCanEverTick = false;
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	RootComponent = MeshComp;
	
	MeshComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	MeshComp->SetCollisionObjectType(ECC_WorldDynamic);
	MeshComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	MeshComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
}

void ABuildPartActor::BeginPlay()
{
	Super::BeginPlay();
	
	TArray<UBoxComponent*> BuildSockets;
	GetComponents<UBoxComponent>(BuildSockets);
	
	for (UBoxComponent* Elem : BuildSockets)
	{
		SocketConnections.Add(Elem, nullptr);
	}
}

bool ABuildPartActor::IsSnapped(ABuildPartActor* BuildPartActor)
{
	for (auto& Elem : SocketConnections)
	{
		if (Elem.Value.IsValid() == true && Elem.Value == BuildPartActor)
			return true;
	}
	return false;
}

UBoxComponent* ABuildPartActor::FindSnapBox(FTransform TF)
{
	FVector Location = TF.GetLocation();
	FQuat Quat = TF.GetRotation();
	
	for (auto& Elem : SocketConnections)
	{
		UBoxComponent* Key = Elem.Key;
		
		if (FVector::DistSquared(Location, Key->GetComponentLocation()) > 0.1f)
			continue;
		
		float Dot = FVector::DotProduct(Key->GetForwardVector(),Quat.GetForwardVector());
		if (Dot < 0.999f)
			continue;
		
		return Key;
	}
	
	return nullptr;
}

bool ABuildPartActor::CanSnapActor(FTransform TF)
{
	UBoxComponent* SnapBox = FindSnapBox(TF);
	if (SnapBox == nullptr)
		return false;
	
	TWeakObjectPtr<ABuildPartActor> SnapActor = *SocketConnections.Find(SnapBox);
	return !SnapActor.IsValid();
}

void ABuildPartActor::Snap(FTransform TF, ABuildPartActor *BuildPartActor)
{
	UBoxComponent* SnapBox = FindSnapBox(TF);
	if (SnapBox == nullptr)
		return;
	
	SocketConnections[SnapBox] = BuildPartActor;
}

