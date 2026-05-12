#pragma once

#include "CoreMinimal.h"
#include "TreeSystem/Interactable.h"
#include "AnimalActor.generated.h"

UCLASS()
class SURVIVAL_API AAnimalActor : public AActor, public IInteractable
{
	GENERATED_BODY()

public:
	AAnimalActor();
	virtual void OnChop(AActor* Chopper) override;

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USkeletalMeshComponent* MeshComp;
};
