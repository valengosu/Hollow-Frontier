#include "BedActor.h"
#include "Camera/CameraComponent.h"
#include "Character/SurvivalCharacter.h"
#include "Engine/DirectionalLight.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "SurvivalComponent/AttributeComponent.h"

ABedActor::ABedActor()
{
	PrimaryActorTick.bCanEverTick = false;
	
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;
	
	BedMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BedMesh"));
	BedMesh->SetupAttachment(RootComponent);
	
	SleepCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("SleepCamera"));
	SleepCamera->SetupAttachment(RootComponent);
}

void ABedActor::BeginPlay()
{
	Super::BeginPlay();
	Sun = Cast<ADirectionalLight>(UGameplayStatics::GetActorOfClass(GetWorld(), ADirectionalLight::StaticClass()));
	StartPitch = Sun->GetActorRotation().Pitch;
}

void ABedActor::StartInteract(AActor* Interactor)
{
	ASurvivalCharacter* Char = Cast<ASurvivalCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	if (Char == nullptr)
		return;
	
	APlayerController* PC = Cast<APlayerController>(Char->GetController());
	if (PC == nullptr)
		return;
	
	Char->DisableInput(PC);
	Char->GetCharacterMovement()->DisableMovement();
	PC->SetViewTargetWithBlend(this, 0.5f);
	
	GetWorldTimerManager().PauseTimer(Char->AttributeComp->PermanentTimerHandle);
	
	SleepTime = 0;
	GetWorld()->GetTimerManager().SetTimer(SleepTimerHandle, this, &ABedActor::UpdateSleepSun, 0.01f, true);
}

void ABedActor::UpdateSleepSun()
{
	SleepTime += 0.01f;
	float Alpha = SleepTime / SleepDuration;
	Alpha = FMath::Min(Alpha, 1);
	
	if (Alpha < 1)
	{
		float CurveValue = SleepSunCurve->GetFloatValue(Alpha);
		FRotator Rot = Sun->GetActorRotation();
		Rot.Pitch = StartPitch + CurveValue;
		
		Sun->SetActorRotation(Rot);
		return;
	}
	
	FRotator Rot = Sun->GetActorRotation();
	Rot.Pitch = StartPitch;
	Sun->SetActorRotation(Rot);
	
	GetWorld()->GetTimerManager().ClearTimer(SleepTimerHandle);
	
	ASurvivalCharacter* Char = Cast<ASurvivalCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	if (Char == nullptr)
		return;
	
	APlayerController* PC = Cast<APlayerController>(Char->GetController());
	if (PC == nullptr)
		return;
		
	Char->EnableInput(PC);
	Char->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	PC->SetViewTargetWithBlend(Char, 0.5f);
	
	GetWorldTimerManager().UnPauseTimer(Char->AttributeComp->PermanentTimerHandle);
}
