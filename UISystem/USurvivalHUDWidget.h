// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BagWidget.h"
#include "RecipeWidget.h"
#include "Components/ProgressBar.h"
#include "Blueprint/UserWidget.h"
#include "USurvivalHUDWidget.generated.h"

struct FOnAttributeChangeData;
/**
 * 
 */
UCLASS()
class SURVIVAL_API USurvivalHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
	void OnHpChange(const FOnAttributeChangeData& Data);
	void OnFoodChange(const FOnAttributeChangeData& Data);
	void OnWaterChange(const FOnAttributeChangeData& Data);
	void OnEnergyChange(const FOnAttributeChangeData& Data);
	
	void AddItemHint(FName RowName);

	UFUNCTION()
	void ToggleInventory();
	
public:
	UPROPERTY(meta = (BindWidget))
	UProgressBar* FoodBar;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* HpBar;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* WaterBar;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* EnergyBar;
	
	UPROPERTY(meta = (BindWidget))
	UBagWidget* BagWidget;
	
	UPROPERTY(meta = (BindWidget))
	URecipeWidget* RecipeWidget;
	
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* DamageImageAnim;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* InteractText;
	
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* AddItemAnim;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemName;
	
	UPROPERTY(meta = (BindWidget))
	UImage* ItemIcon;
};
