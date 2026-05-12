#include "UIManagerSubsystem.h"
#include "Character/SurvivalCharacter.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Config/ItemConfig.h"
#include "Config/SurvivalGI.h"
#include "SurvivalComponent/InventoryComponent.h"

void UUIManagerSubsystem::CreateUI(APlayerController* PC)
{
	USurvivalGI* GI = Cast<USurvivalGI>(GetLocalPlayer()->GetGameInstance());
	SurvivalHUDWidget = CreateWidget<USurvivalHUDWidget>(PC, GI->StatsWidgetClass);	
	SurvivalHUDWidget->AddToViewport();	
}

void UUIManagerSubsystem::AddItem(FName RowName, int Count)
{
	if (GetCharacter() == nullptr)
		return;
	
	GetCharacter()->InventoryComp->AddItem(RowName, Count);
	SurvivalHUDWidget->BagWidget->RenderItems(GetCharacter()->InventoryComp->Items);
	
	FItemConfig* Row = GetTableRow<FItemConfig>(this, RowName);
	SurvivalHUDWidget->ItemIcon->SetBrushFromTexture(Row->Icon, true);
	SurvivalHUDWidget->ItemName->SetText(FText::FromName(RowName));
	
	SurvivalHUDWidget->PlayAnimation(SurvivalHUDWidget->AddItemAnim);
}

bool UUIManagerSubsystem::RemoveItem(FName RowName, int Count)
{
	if (GetCharacter() == nullptr)
		return false;
	
	if (GetCharacter()->InventoryComp->RemoveItem(RowName, Count) == false)
		return false;
	
	SurvivalHUDWidget->BagWidget->RenderItems(GetCharacter()->InventoryComp->Items);
	return true;
}

ASurvivalCharacter* UUIManagerSubsystem::GetCharacter()
{
	if (!CachedPawn.IsValid())
	{
		if (APlayerController* PC = GetLocalPlayer()->GetPlayerController(GetWorld()))
		{
			CachedPawn = Cast<ASurvivalCharacter>(PC->GetPawn());
		}
	}
	return CachedPawn.Get();
}
