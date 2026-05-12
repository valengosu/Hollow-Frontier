#include "RecipeWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Config/ItemConfig.h"
#include "Config/SurvivalGI.h"
#include "GameFramework/Character.h"
#include "SurvivalComponent/InventoryComponent.h"

class UInventoryComponent;

void URecipeWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	USurvivalGI* GI = Cast<USurvivalGI>(GetWorld()->GetGameInstance());
	for (FName ItemName : GI->RecipeItem)
	{
		UGridWidget* GridWidget = CreateWidget<UGridWidget>(GetOwningPlayer(), GridWidgetClass);
		GridWidget->Count->SetVisibility(ESlateVisibility::Collapsed);
		
		FItemConfig* Row = GetTableRow<FItemConfig>(this, ItemName);
		GridWidget->Icon->SetBrushFromTexture(Row->Icon);
		GridWidget->ItemName = ItemName;
		GridWidget->GridButton->OnClicked.AddDynamic(GridWidget, &UGridWidget::GridWidgetClicked);
		GridWidget->OnGridWidgetClicked.AddDynamic(this, &URecipeWidget::ShowRecipeResourceWidgets);
		
		WrapBox->AddChildToWrapBox(GridWidget);
		
		FButtonStyle Style = GridWidget->GridButton->GetStyle();
		Style.Normal.TintColor = FSlateColor(FLinearColor(0.f, 0.099f, 0.12f, 0.699f));
		GridWidget->GridButton->SetStyle(Style);
	}
	
	CraftButton->OnClicked.AddDynamic(this, &URecipeWidget::TryCraftItem);
}

void URecipeWidget::ShowRecipeResourceWidgets(FName ItemName)
{
	VerticalBox->ClearChildren();
	
	CraftPanel->SetVisibility(ESlateVisibility::Visible);
	CraftButton->SetVisibility(ESlateVisibility::Visible);
	CraftButton->SetIsEnabled(true);
	CurrentCraftItemName = ItemName;
	
	USurvivalGI* GI = Cast<USurvivalGI>(GetWorld()->GetGameInstance());
	TArray<FName> ResourceItemNames;
	TArray<int> ResourceItemCounts;
	GI->GetRecipeResourceItems(ItemName, ResourceItemNames, ResourceItemCounts);
	
	APawn* Pawn = GetOwningPlayerPawn();
	ACharacter* Character = Cast<ACharacter>(Pawn);
	UInventoryComponent* Inventory = Character->FindComponentByClass<UInventoryComponent>();
	
	for (int i = 0; i < ResourceItemNames.Num(); i++)
	{
		UGridWidget* GridWidget = CreateWidget<UGridWidget>(GetOwningPlayer(), GridWidgetClass);
		
		FItemConfig* Row = GetTableRow<FItemConfig>(this, ResourceItemNames[i]);
		GridWidget->Icon->SetBrushFromTexture(Row->Icon);
		
		int Count = Inventory->GetItemCount(ResourceItemNames[i]);
		GridWidget->Count->SetText(FText::AsNumber(Count));
		if (Count < ResourceItemCounts[i])
		{
			GridWidget->Count->SetColorAndOpacity(FSlateColor(FLinearColor::Red));
			CraftButton->SetIsEnabled(false);
			CurrentCraftItemName = NAME_None;
		}
		
		VerticalBox->AddChild(GridWidget);
		
		FButtonStyle Style = GridWidget->GridButton->GetStyle();
		Style.Normal.TintColor = FSlateColor(FLinearColor(0.12f, 0.12f, 0.12f, 0.f));
		GridWidget->GridButton->SetStyle(Style);
	}
}

void URecipeWidget::TryCraftItem()
{
	APawn* Pawn = GetOwningPlayerPawn();
	ACharacter* Character = Cast<ACharacter>(Pawn);
	UInventoryComponent* Inventory = Character->FindComponentByClass<UInventoryComponent>();
	
	if (Inventory->CraftItem(CurrentCraftItemName) == true)
	{
		ShowRecipeResourceWidgets(CurrentCraftItemName);
	}
}
