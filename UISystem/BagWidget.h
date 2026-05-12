// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WrapBox.h"
#include "Blueprint/UserWidget.h"
#include "BagWidget.generated.h"

struct FItemConfig;
class UGridWidget;
struct FItem;

UCLASS()
class SURVIVAL_API UBagWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void RenderItems(const TArray<FItem> &Items);
	virtual void NativeConstruct() override;
	
	UFUNCTION()
	void BindCallback(UGridWidget* GridWidget, const FItemConfig& Row);
	
	UFUNCTION()
	void EquipItem(FName ItemName);
	
	UFUNCTION()
	void StartBuild(FName ItemName);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UGridWidget> GridWidgetClass;
	
	UPROPERTY(meta = (BindWidget))
	UWrapBox* WrapBox;
};
