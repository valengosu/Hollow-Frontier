// Copyright Epic Games, Inc. All Rights Reserved.

#include "SurvivalCharacter.h"
#include "AbilitySystemComponent.h"
#include "../Attribute/Attribute.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Survival.h"
#include "BuildSystem/BuildSubsystem.h"
#include "Config/ItemConfig.h"
#include "Config/SurvivalGI.h"
#include "Engine/StaticMeshActor.h"
#include "SurvivalComponent/AttributeComponent.h"
#include "SurvivalComponent/InteractionComponent.h"
#include "SurvivalComponent/InventoryComponent.h"
#include "UISystem//USurvivalHUDWidget.h"
#include "UISystem/UIManagerSubsystem.h"

class UFoliageInstancedStaticMeshComponent;

UAbilitySystemComponent* ASurvivalCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ASurvivalCharacter::Die()
{
	GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
	GetMesh()->SetSimulatePhysics(true);
	DisableInput(Cast<APlayerController>(GetController()));
	GetCharacterMovement()->DisableMovement();
}

void ASurvivalCharacter::HoldingItem(FName ItemName)
{
	if (HoldItem != nullptr)
		HoldItem->Destroy();
	
	FItemConfig* Row = GetTableRow<FItemConfig>(this, ItemName);
	if (Row == nullptr)
		return;
	
	HoldItem = GetWorld()->SpawnActor<AStaticMeshActor>(Row->StaticMeshActorClass, FVector::ZeroVector, FRotator::ZeroRotator);
	UStaticMeshComponent* ItemMesh = HoldItem->GetStaticMeshComponent();
	ItemMesh->SetSimulatePhysics(false);
	ItemMesh->SetCollisionProfileName(TEXT("NoCollision"));
	HoldItem->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("HandGrip_R"));
	
	IsHold = true;
}

void ASurvivalCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ASurvivalCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	AbilitySystemComponent->InitAbilityActorInfo(GetOwner(), GetOwner());
	
	AttributeComp->Attribute = Attribute;
	AttributeComp->AbilitySystemComponent = AbilitySystemComponent;
	
	APlayerController* PC = Cast<APlayerController>(NewController);
	UUIManagerSubsystem* UIManagerSubsystem = PC->GetLocalPlayer()->GetSubsystem<UUIManagerSubsystem>();
	UIManagerSubsystem->CreateUI(PC);
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UAttribute::GetHpAttribute()).AddUObject(UIManagerSubsystem->SurvivalHUDWidget, &USurvivalHUDWidget::OnHpChange);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UAttribute::GetFoodAttribute()).AddUObject(UIManagerSubsystem->SurvivalHUDWidget, &USurvivalHUDWidget::OnFoodChange);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UAttribute::GetWaterAttribute()).AddUObject(UIManagerSubsystem->SurvivalHUDWidget, &USurvivalHUDWidget::OnWaterChange);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UAttribute::GetEnergyAttribute()).AddUObject(UIManagerSubsystem->SurvivalHUDWidget, &USurvivalHUDWidget::OnEnergyChange);
	
	GetWorld()->GetTimerManager().SetTimer(AttributeComp->PermanentTimerHandle, AttributeComp, &UAttributeComponent::OnTimerTick, 1.0f, true);
}

ASurvivalCharacter::ASurvivalCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetMesh(), TEXT("head"));
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
	FollowCamera->PostProcessSettings.MotionBlurAmount = 0.0f;
	FollowCamera->PostProcessSettings.bOverride_MotionBlurAmount = true;
	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
	
	Attribute = CreateDefaultSubobject<UAttribute>(FName("MyAttributeSet"));
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>("MyAbilitySystemComponent");
	
	AttributeComp = CreateDefaultSubobject<UAttributeComponent>(TEXT("AttributeComponent"));
	InventoryComp = CreateDefaultSubobject<UInventoryComponent>("InventoryComponent");
	InteractionComp = CreateDefaultSubobject<UInteractionComponent>("InteractionComponent");
}

void ASurvivalCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASurvivalCharacter::Move);
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &ASurvivalCharacter::Look);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASurvivalCharacter::Look);
		
		EnhancedInputComponent->BindAction(PrimaryAction, ETriggerEvent::Started, this, &ASurvivalCharacter::PlaceBuildPart);
		EnhancedInputComponent->BindAction(PrimaryAction, ETriggerEvent::Started, this, &ASurvivalCharacter::Chop);
		EnhancedInputComponent->BindAction(ToggleInventoryAction, ETriggerEvent::Started, this, &ASurvivalCharacter::ToggleInventory);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this->InteractionComp, &UInteractionComponent::StartInteraction);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this->InteractionComp, &UInteractionComponent::Interacting);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Completed, this->InteractionComp, &UInteractionComponent::EndInteraction);
		EnhancedInputComponent->BindAction(CancelAction, ETriggerEvent::Started, this, &ASurvivalCharacter::HandleCancel);
	}
	else
	{
		UE_LOG(LogSurvival, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ASurvivalCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	// route the input
	DoMove(MovementVector.X, MovementVector.Y);
}

void ASurvivalCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	// route the input
	DoLook(LookAxisVector.X, LookAxisVector.Y);
}

void ASurvivalCharacter::DoMove(float Right, float Forward)
{
	if (GetController() != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = GetController()->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, Forward);
		AddMovementInput(RightDirection, Right);
	}
}

void ASurvivalCharacter::DoLook(float Yaw, float Pitch)
{
	if (GetController() != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(Yaw);
		AddControllerPitchInput(Pitch);
	}
}

void ASurvivalCharacter::DoJumpStart()
{
	// signal the character to jump
	Jump();
}

void ASurvivalCharacter::DoJumpEnd()
{
	// signal the character to stop jumping
	StopJumping();
}

void ASurvivalCharacter::Chop()
{
	if (HoldItem == nullptr)
		return;
	
	USurvivalGI* GI = Cast<USurvivalGI>(GetWorld()->GetGameInstance());	
	
	if (GetMesh()->GetAnimInstance()->Montage_IsPlaying(GI->SkillMontage) == true)
		return;

	PlayAnimMontage(GI->SkillMontage);
}

void ASurvivalCharacter::ToggleInventory()
{
	UUIManagerSubsystem* UIManagerSubsystem = GetController<APlayerController>()->GetLocalPlayer()->GetSubsystem<UUIManagerSubsystem>();
	UIManagerSubsystem->SurvivalHUDWidget->ToggleInventory();
}

void ASurvivalCharacter::PlaceBuildPart()
{
	UBuildSubsystem* Subsystem = GetController<APlayerController>()->GetLocalPlayer()->GetSubsystem<UBuildSubsystem>();
	Subsystem->PlaceBuildPartActor();
}

void ASurvivalCharacter::HandleCancel()
{
	return;
	/*
	//UE_LOG(LogTemp, Warning, TEXT("Hello"));
	UUIManagerSubsystem* UIManagerSubsystem = GetController<APlayerController>()->GetLocalPlayer()->GetSubsystem<UUIManagerSubsystem>();
	if (UIManagerSubsystem->SurvivalHUDWidget->BagWidget->GetVisibility() == ESlateVisibility::Collapsed)
		return;
	
	UBuildSubsystem* Subsystem = GetController<APlayerController>()->GetLocalPlayer()->GetSubsystem<UBuildSubsystem>();
	Subsystem->ActiveBuildMode(false);
	Subsystem->Demolish();
	*/
}
