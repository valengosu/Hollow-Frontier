#include "InventoryComponent.h"
#include "Config/ItemConfig.h"
#include "Config/SurvivalGI.h"
#include "UISystem/UIManagerSubsystem.h"

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	
	APlayerController* PC = GetOwner<APawn>()->GetController<APlayerController>();
	UUIManagerSubsystem* UIManagerSubsystem = PC->GetLocalPlayer()->GetSubsystem<UUIManagerSubsystem>();
	UIManagerSubsystem->AddItem("Wood", 5);
	UIManagerSubsystem->AddItem("Stone", 11);
	UIManagerSubsystem->AddItem("Axe", 1);
	UIManagerSubsystem->AddItem("Wall", 5);
	UIManagerSubsystem->AddItem("Floor", 5);
	UIManagerSubsystem->AddItem("Window", 5);
	UIManagerSubsystem->AddItem("Door", 5);
}

void UInventoryComponent::AddItem(FName RowName, int Count)
{
	FItemConfig* Row = GetTableRow<FItemConfig>(this, RowName);
	if (Row == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("ItemConfig: %s"), *RowName.ToString());
		return;
	}
	
	for (FItem& Item : Items)
	{
		if (Count == 0)
			break;
		
		if (Item.RowName == RowName)
		{
			int DiffCount = Row->MaxPackCount - Item.Count; 
			int AddCount = DiffCount > Count ? Count : DiffCount;
			Item.Count += AddCount;
			Count -= AddCount;	
		}
	}
	
	while(Count > 0)
	{
		int AddCount = Count > Row->MaxPackCount ? Row->MaxPackCount : Count; 
		Items.Add({RowName, AddCount});
		Count -= AddCount;
	}
}

bool UInventoryComponent::RemoveItem(FName RowName, int Count)
{
	int TotalCount = GetItemCount(RowName);
	if (TotalCount < Count)
		return false;
	
	for (int32 i = Items.Num() - 1; i >= 0; i--)
	{
		if (Items[i].RowName != RowName)
			continue;
		
		if (Count >= Items[i].Count)
		{
			Count -= Items[i].Count;
			Items.RemoveAt(i);
			continue;
		}
		
		Items[i].Count -= Count;
		break;
	}
	
	return true;
}

int UInventoryComponent::GetItemCount(FName RowName)
{
	int Count = 0; 
	for (FItem& Item : Items)
	{
		if (Item.RowName == RowName)
		{
			Count += Item.Count;
		}
	}
	return Count;
}

bool UInventoryComponent::CraftItem(FName ItemName)
{
	USurvivalGI* GI = Cast<USurvivalGI>(GetWorld()->GetGameInstance());
	TArray<FName> ResourceItemNames;
	TArray<int> ResourceItemCounts;
	GI->GetRecipeResourceItems(ItemName, ResourceItemNames, ResourceItemCounts);
	
	for (int i = 0; i < ResourceItemNames.Num(); i++)
	{
		int Count = GetItemCount(ResourceItemNames[i]);
		if (Count < ResourceItemCounts[i])
		{
			return false;
		}
	}
	
	for (int i = 0; i < ResourceItemNames.Num(); i++)
	{
		RemoveItem(ResourceItemNames[i], ResourceItemCounts[i]);
	}
	
	AddItem(ItemName, 1);
	return true;
}
