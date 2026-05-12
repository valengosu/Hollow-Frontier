#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractionComponent.generated.h"

class ABuildPartActor;
class IInteractable;
class ABedActor;
class AWaterBodyOcean;
class AABuildPreviewActor;

enum class EBuildPlacementType
{
	None,
	Snap,
	Invalid
};

struct FHitInfo
{
	FHitInfo()
	{
		IsHit = false;
		EndLocation = FVector::ZeroVector;
		HitActor = nullptr;
		HitComponent = nullptr;
	}
	
	bool IsHit;
	FVector EndLocation;
	AActor* HitActor;
	UPrimitiveComponent* HitComponent;
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SURVIVAL_API UInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UInteractionComponent();
	
	bool IsLookingAtBed();
	bool IsUnderWater();
	void StartInteraction();
	void Interacting();
	void EndInteraction();
	FHitInfo GetNearestHitInfo(FVector StartLocation, const FHitInfo &HitA, const FHitInfo &HitB);
	
	void DrinkWater();
	void StopDrinkWater();
	EBuildPlacementType PreviewBuildPosition(AABuildPreviewActor* PreviewActor);
	FHitInfo GetHitInfo(const FVector &StartPoint, const FVector &Dir, float Length, ECollisionChannel Channel, FColor Color = FColor::Cyan);
	FHitInfo CameraTraceDection(float Length, ECollisionChannel Channel, FColor Color = FColor::Cyan);
	
protected:
	virtual void BeginPlay() override;
	
private:
	UPROPERTY()
	FTimerHandle DrinkTimerHandle;
	
	UPROPERTY(EditAnywhere)
	UAudioComponent* AudioComp;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<AActor> InteractActor;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<ABuildPartActor> BuildPartFloorClass;
};
