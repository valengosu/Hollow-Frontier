// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "USurvivalHUDWidget.h"
#include "Subsystems/LocalPlayerSubsystem.h"
#include "UIManagerSubsystem.generated.h"


class ASurvivalCharacter;

/**
 * 
 */
UCLASS(Blueprintable)
class SURVIVAL_API UUIManagerSubsystem : public ULocalPlayerSubsystem
{
	GENERATED_BODY()
	
public:
	void CreateUI(APlayerController *PC);
	void AddItem(FName RowName, int Count);
	bool RemoveItem(FName RowName, int Count);
	ASurvivalCharacter* GetCharacter();
	
public:
	UPROPERTY()
	USurvivalHUDWidget* SurvivalHUDWidget;

private:
	TWeakObjectPtr<ASurvivalCharacter> CachedPawn;
};
