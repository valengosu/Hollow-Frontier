// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GridWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGridWidgetClicked, FName, ItemName);

class UButton;
class UImage;
class UTextBlock;

UCLASS()
class SURVIVAL_API UGridWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION()
	void ChangeColor(FSlateColor Color);
	
	UPROPERTY(meta = (BindWidget))
	UButton* GridButton;
	
	UPROPERTY(meta = (BindWidget))
	UImage* Icon;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Count;
	
	UFUNCTION()
	void GridWidgetClicked();
	
	UPROPERTY(BlueprintAssignable)
	FOnGridWidgetClicked OnGridWidgetClicked;
	
	FName ItemName;
};
