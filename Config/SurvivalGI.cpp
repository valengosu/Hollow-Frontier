#include "SurvivalGI.h"
#include "WaterBodyOceanActor.h"
#include "Kismet/GameplayStatics.h"
#include "Config/RecipeConfig.h"
#include "UISystem/USurvivalHUDWidget.h"

void USurvivalGI::Init()
{
	Super::Init();
	
	for (UDataTable* Table : AllTables)
	{
		const UScriptStruct* RowStruct = Table->GetRowStruct();
		TableStructs.Add(RowStruct, Table);
		
		if (RowStruct == FRecipeConfig::StaticStruct())
		{
			Table->GetAllRows(TEXT("RecipeTable"), RecipeConfigRows);
			for (FRecipeConfig* Recipe : RecipeConfigRows)
			{
				RecipeItem.Add(Recipe->ItemName);
			}
		}
	}
}

AWaterBodyOcean* USurvivalGI::GetOcean()
{
	if (Ocean == nullptr)
	{
		Ocean = Cast<AWaterBodyOcean>(UGameplayStatics::GetActorOfClass(GetWorld(), AWaterBodyOcean::StaticClass()));	
	}
	return Ocean;
}

void USurvivalGI::GetRecipeResourceItems(FName ItemName, TArray<FName>& ResourceItemNames, TArray<int>& ResourceItemCounts)
{
	for (FRecipeConfig* Recipe : RecipeConfigRows)
	{
		if (Recipe->ItemName == ItemName)
		{
			ResourceItemNames.Add(Recipe->ResourceItemName);
			ResourceItemCounts.Add(Recipe->Count);
		}
	}
}
