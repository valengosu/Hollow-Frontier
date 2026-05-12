#include "USurvivalHUDWidget.h"
#include "Attribute/Attribute.h"
#include "Attribute/ChangeAttributeCalculation.h"
#include "Character/SurvivalCharacter.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Config/ItemConfig.h"
#include "Config/SurvivalGI.h"
#include "SurvivalComponent/InteractionComponent.h"

void USurvivalHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	HpBar->SetPercent(1.0);
	FoodBar->SetPercent(1.0);
	WaterBar->SetPercent(1.0);
	EnergyBar->SetPercent(1.0);
	
	BagWidget->SetVisibility(ESlateVisibility::Collapsed);
	
	RecipeWidget->SetVisibility(ESlateVisibility::Collapsed);
	RecipeWidget->CraftPanel->SetVisibility(ESlateVisibility::Collapsed);
	RecipeWidget->CraftButton->SetVisibility(ESlateVisibility::Collapsed);
}

void USurvivalHUDWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
	InteractText->SetVisibility(ESlateVisibility::Hidden);
	ASurvivalCharacter* Character = Cast<ASurvivalCharacter>(GetOwningPlayerPawn());
	
	if (Character->InteractionComp->IsLookingAtBed() == true)
	{
		InteractText->SetText(FText::FromString("Press E : Sleep"));
		InteractText->SetVisibility(ESlateVisibility::Visible);
	}
	
	if (Character->InteractionComp->IsUnderWater() == true)
	{
		InteractText->SetText(FText::FromString("Hold E : Drink"));
		InteractText->SetVisibility(ESlateVisibility::Visible);
	}
}

void USurvivalHUDWidget::OnHpChange(const FOnAttributeChangeData& Data)
{
	HpBar->SetPercent((float)Data.NewValue / (float)UAttribute::MaxValue);
	
	if (Data.NewValue < Data.OldValue && Data.NewValue != 0)
	{
		PlayAnimation(DamageImageAnim);
	}
}

void USurvivalHUDWidget::OnFoodChange(const FOnAttributeChangeData& Data)
{
	FoodBar->SetPercent((float)Data.NewValue / (float)UAttribute::MaxValue);
}

void USurvivalHUDWidget::OnWaterChange(const FOnAttributeChangeData& Data)
{
	WaterBar->SetPercent((float)Data.NewValue / (float)UAttribute::MaxValue);
}

void USurvivalHUDWidget::OnEnergyChange(const FOnAttributeChangeData& Data)
{
	EnergyBar->SetPercent((float)Data.NewValue / (float)UAttribute::MaxValue);
}

void USurvivalHUDWidget::AddItemHint(FName RowName)
{
	FItemConfig* Row = GetTableRow<FItemConfig>(this, RowName);
	if (Row == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("ItemConfig: %s"), *RowName.ToString());
		return;
	}
	
	ItemName->SetText(FText::FromName(RowName));
	ItemIcon->SetBrushFromTexture(Row->Icon, true);
	
	PlayAnimation(AddItemAnim);
}

void USurvivalHUDWidget::ToggleInventory()
{
	if (BagWidget->GetVisibility() == ESlateVisibility::Collapsed)
	{
		BagWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		
		RecipeWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		RecipeWidget->CraftPanel->SetVisibility(ESlateVisibility::Collapsed);
		RecipeWidget->CraftButton->SetVisibility(ESlateVisibility::Collapsed);
		
		FInputModeGameAndUI InputMode;
		GetOwningPlayer()->SetInputMode(InputMode);
		GetOwningPlayer()->SetShowMouseCursor(true);
	}
	else //BagWidget->GetVisibility() == ESlateVisibility::Visible
	{
		BagWidget->SetVisibility(ESlateVisibility::Collapsed);
		
		RecipeWidget->SetVisibility(ESlateVisibility::Collapsed);
		RecipeWidget->CraftPanel->SetVisibility(ESlateVisibility::Collapsed);
		RecipeWidget->CraftButton->SetVisibility(ESlateVisibility::Collapsed);
		
		FInputModeGameOnly InputMode;
		GetOwningPlayer()->SetInputMode(InputMode);
		GetOwningPlayer()->SetShowMouseCursor(false);
	}
}
