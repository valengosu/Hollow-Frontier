#pragma once

#include "CoreMinimal.h"
#include "TreeSystem/Interactable.h"
#include "BedActor.generated.h"

class ADirectionalLight;
class UCameraComponent;

UCLASS()
class SURVIVAL_API ABedActor : public AActor, public IInteractable
{
	GENERATED_BODY()

public:
	ABedActor();

protected:
	virtual void BeginPlay() override;
	virtual void StartInteract(AActor* Interactor) override;
	void UpdateSleepSun();
	
	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UCameraComponent* SleepCamera;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* BedMesh;
	
	UPROPERTY(EditAnywhere)
	UCurveFloat* SleepSunCurve;
	
	UPROPERTY()
	FTimerHandle SleepTimerHandle;
	
	UPROPERTY()
	ADirectionalLight* Sun;
	
private:
	float SleepTime = 0.f;
	float SleepDuration = 12.f;
	float StartPitch;
};
