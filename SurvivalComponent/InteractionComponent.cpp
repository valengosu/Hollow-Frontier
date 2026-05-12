#include "InteractionComponent.h"
#include "AttributeComponent.h"
#include "GameFramework/Character.h"
#include "Attribute/Attribute.h"
#include "Attribute/GameplayTag.h"
#include "BuildSystem/ABuildPreviewActor.h"
#include "BuildSystem/BuildPartActor.h"
#include "Character/SurvivalCharacter.h"
#include "Components/AudioComponent.h"
#include "Components/BoxComponent.h"
#include "Config/SurvivalGI.h"
#include "TreeSystem/Interactable.h"
#include "WorldActor/BedActor.h"

UInteractionComponent::UInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	AudioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComp"));
}

void UInteractionComponent::BeginPlay()
{
	Super::BeginPlay();
}

FHitInfo UInteractionComponent::CameraTraceDection(float Length, ECollisionChannel Channel, FColor Color)
{
	FVector CameraPos;
	FRotator CameraRotation;
	
	APlayerController* PC = Cast<APlayerController>(Cast<ACharacter>(GetOwner())->GetController());
	PC->GetPlayerViewPoint(CameraPos, CameraRotation);
	
	return GetHitInfo(CameraPos, CameraRotation.Vector(), Length, Channel, Color);
}

FHitInfo UInteractionComponent::GetHitInfo(const FVector &StartPoint, const FVector &Dir, float Length, ECollisionChannel Channel, FColor Color)
{
	//DrawDebugLine(GetWorld(), CameraPos,EndPos, Color, false, 0.f, 0,2.f);
	FHitInfo Info;
	Info.EndLocation = StartPoint + Dir * Length; 
	
	FHitResult Hit;
	bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, StartPoint, Info.EndLocation, Channel);
	if (bHit == true)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Channel %s"), *UEnum::GetValueAsString(Channel));
		//UE_LOG(LogTemp, Warning, TEXT("Point %f"), EndPos.Z);
		//DrawDebugSphere(GetWorld(), EndPos, 10.f, 12, Color, false, 2.f);
		
		Info.IsHit = true;
		Info.EndLocation = Hit.ImpactPoint;
		Info.HitActor = Hit.GetActor();
		Info.HitComponent = Hit.GetComponent();
		
		if (ABuildPartActor *Part = Cast<ABuildPartActor>(Hit.GetActor()))
		{
			Info.EndLocation += Hit.ImpactNormal * 0.005f;
		}
	}
	
	return Info;
}

FHitInfo UInteractionComponent::GetNearestHitInfo(FVector StartLocation, const FHitInfo &HitA, const FHitInfo &HitB)
{
	if (HitA.IsHit == false && HitB.IsHit == false)
		return FHitInfo();
	
	if (HitA.IsHit == true && HitB.IsHit == false)
		return HitA;
	
	if (HitA.IsHit == false && HitB.IsHit == true)
		return HitB;
	
	float DistA = FVector::DistSquared(StartLocation, HitA.EndLocation);
	float DistB = FVector::DistSquared(StartLocation, HitB.EndLocation);
	
	return DistA <= DistB ? HitA : HitB;
}

EBuildPlacementType UInteractionComponent::PreviewBuildPosition(AABuildPreviewActor* PreviewActor)
{
	PreviewActor->SnapActor = nullptr;
	PreviewActor->SnapComponent = nullptr;
	
	APlayerController* PC = Cast<APlayerController>(Cast<ACharacter>(GetOwner())->GetController());
	
	float MouseX, MouseY;
	PC->GetMousePosition(MouseX, MouseY);
	
	FVector WorldLocation;
	FVector WorldDirection;
	PC->DeprojectScreenPositionToWorld(MouseX, MouseY,WorldLocation,WorldDirection);
	
	FHitInfo VisibilityHitInfo = GetHitInfo(WorldLocation, WorldDirection, 1200, ECC_Visibility);
	FHitInfo SocketHitInfo = GetHitInfo(WorldLocation, WorldDirection, 1200, UEngineTypes::ConvertToCollisionChannel(PreviewActor->SocketChannel));
	FHitInfo HitInfo = GetNearestHitInfo(WorldLocation, VisibilityHitInfo, SocketHitInfo);
	
	if (HitInfo.IsHit == false)
	{
		return EBuildPlacementType::Invalid;
	}
	
	FVector Location = HitInfo.EndLocation;
	if (PreviewActor->BuildPartActorClass != BuildPartFloorClass)
	{
		Location += FVector(0, 0, 200);
	}
	PreviewActor->SetActorLocation(Location);
	
	UBoxComponent* SnapComponent = Cast<UBoxComponent>(HitInfo.HitComponent);
	
	if (SnapComponent == nullptr || Cast<ABuildPartActor>(SnapComponent->GetOwner()) == nullptr)
	{
		return EBuildPlacementType::None;
	}
	
	PreviewActor->SnapActor = Cast<ABuildPartActor>(SnapComponent->GetOwner());
	PreviewActor->SnapComponent = HitInfo.HitComponent;
		
	Location = HitInfo.HitComponent->GetComponentTransform().GetLocation();
	FQuat Quat = HitInfo.HitComponent->GetComponentTransform().GetRotation();
	PreviewActor->SetActorLocationAndRotation(Location, Quat);
	
	return EBuildPlacementType::Snap;
}

bool UInteractionComponent::IsLookingAtBed()
{
	FHitInfo HitInfo = CameraTraceDection(200.f, ECC_Visibility);
	return Cast<ABedActor>(HitInfo.HitActor) != nullptr;
}

bool UInteractionComponent::IsUnderWater()
{
	FHitInfo HitInfo = CameraTraceDection(200.f, ECC_Visibility, FColor::Red);
	
	USurvivalGI* GI = Cast<USurvivalGI>(GetWorld()->GetGameInstance());
	float WaterHeight;
	FVector WaterPos;
	FVector WaterNormal;
	FVector WaterVelocity;
	GI->GetOcean()->GetWaterBodyComponent()->GetWaterSurfaceInfoAtLocation(HitInfo.EndLocation, WaterPos,WaterNormal, WaterVelocity, WaterHeight);
	//UE_LOG(LogTemp, Warning, TEXT("%f"), WaterHeight);
	
	return HitInfo.EndLocation.Z < WaterPos.Z;
}

void UInteractionComponent::StartInteraction()
{
	if (IsUnderWater() == true)
	{
		AudioComp->Play(0);
		AudioComp->SetPitchMultiplier(0.01f);
		GetWorld()->GetTimerManager().SetTimer(DrinkTimerHandle, this, &UInteractionComponent::DrinkWater, 0.1f, true);
		return;
	}
	
	FHitInfo HitInfo = CameraTraceDection(200.f, ECC_Visibility);
	
	if (IInteractable* Interact = Cast<IInteractable>(HitInfo.HitActor))
	{
		InteractActor = HitInfo.HitActor;
		Interact->StartInteract(GetOwner());
		return;
	}
}

void UInteractionComponent::Interacting()
{
	//UE_LOG(LogTemp, Warning, TEXT("Hello"));
	if (IsUnderWater() == true && GetWorld()->GetTimerManager().TimerExists(DrinkTimerHandle) == false)
	{
		AudioComp->Play(0);
		AudioComp->SetPitchMultiplier(0.01f);
		GetWorld()->GetTimerManager().SetTimer(DrinkTimerHandle, this, &UInteractionComponent::DrinkWater, 0.1f, true);
	}
}

void UInteractionComponent::DrinkWater()
{
	if (IsUnderWater() == false)
	{
		StopDrinkWater();
		return;
	}
	
    ASurvivalCharacter* Char = Cast<ASurvivalCharacter>(GetOwner());
	Char->AttributeComp->Attribute->ChangeAttribute(TAG_Attribute_Water, 1.0);
}

void UInteractionComponent::StopDrinkWater()
{
	AudioComp->Stop();
	GetWorld()->GetTimerManager().ClearTimer(DrinkTimerHandle);
}

void UInteractionComponent::EndInteraction()
{
	if (IsUnderWater() == true)
	{
		AudioComp->Stop();
		GetWorld()->GetTimerManager().ClearTimer(DrinkTimerHandle);
		return;
	}
	
	if (IsValid(InteractActor) == true)
	{
		Cast<IInteractable>(InteractActor)->EndInteract(GetOwner());
	}
}

