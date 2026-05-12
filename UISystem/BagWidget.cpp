#include "BagWidget.h"
#include "GridWidget.h"
#include "IDetailTreeNode.h"
#include "UIManagerSubsystem.h"
#include "../Character/SurvivalCharacter.h"
#include "BuildSystem/BuildSubsystem.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Config/BuildPartConfig.h"
#include "Config/ItemConfig.h"
#include "Config/SurvivalGI.h"
#include "Engine/StaticMeshActor.h"
#include "GameFramework/Character.h"
#include "SurvivalComponent/InventoryComponent.h"

void UBagWidget::RenderItems(const TArray<FItem> &Items)
{
	WrapBox->ClearChildren();
	
	for (const FItem& Item : Items)
	{
		UGridWidget* GridWidget = CreateWidget<UGridWidget>(GetOwningPlayer(), GridWidgetClass);
		GridWidget->Count->SetText(FText::AsNumber(Item.Count));
		
		FItemConfig* Row = GetTableRow<FItemConfig>(this, Item.RowName);
		GridWidget->Icon->SetBrushFromTexture(Row->Icon);
		GridWidget->ItemName = Item.RowName;
		GridWidget->GridButton->OnClicked.AddDynamic(GridWidget, &UGridWidget::GridWidgetClicked);
		BindCallback(GridWidget, *Row);
		
		WrapBox->AddChildToWrapBox(GridWidget);
		
		FButtonStyle Style = GridWidget->GridButton->GetStyle();
		Style.Normal.TintColor = FSlateColor(FLinearColor(0.12f, 0.12f, 0.12f, 0.7f));
		GridWidget->GridButton->SetStyle(Style);
	}
}

void UBagWidget::BindCallback(UGridWidget* GridWidget, const FItemConfig& Row)
{
	if (Row.StaticMeshActorClass != nullptr)
	{
		GridWidget->OnGridWidgetClicked.AddDynamic(this, &UBagWidget::EquipItem);
		return;
	}
	
	if (GetTableRow<FBuildPartConfig>(this, GridWidget->ItemName) != nullptr)
	{
		GridWidget->OnGridWidgetClicked.AddDynamic(this, &UBagWidget::StartBuild);
		return;
	}
}

void UBagWidget::EquipItem(FName ItemName)
{
	Cast<ASurvivalCharacter>(GetOwningPlayerPawn())->HoldingItem(ItemName);
}

void UBagWidget::StartBuild(FName ItemName)
{
	ULocalPlayer* LP = GetOwningLocalPlayer();
	UBuildSubsystem* Subsystem = LP->GetSubsystem<UBuildSubsystem>();
	Subsystem->ActiveBuildMode(true, ItemName); 
}

void UBagWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	APawn* Pawn = GetOwningPlayerPawn();
	ACharacter* Character = Cast<ACharacter>(Pawn);
	UInventoryComponent* Inventory = Character->FindComponentByClass<UInventoryComponent>();
	
	RenderItems(Inventory->Items);
}

