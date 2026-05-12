#include "AnimalActor.h"

#include "Character/SurvivalCharacter.h"
#include "UISystem/UIManagerSubsystem.h"

AAnimalActor::AAnimalActor()
{
	PrimaryActorTick.bCanEverTick = false;
	
	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);
}

void AAnimalActor::OnChop(AActor* Chopper)
{
	if (ASurvivalCharacter* Char = Cast<ASurvivalCharacter>(Chopper))
	{
		UUIManagerSubsystem* UIManagerSubsystem = Char->GetController<APlayerController>()->GetLocalPlayer()->GetSubsystem<UUIManagerSubsystem>();
		UIManagerSubsystem->AddItem("Meat",20);
	}
}

void AAnimalActor::BeginPlay()
{
	Super::BeginPlay();
}



