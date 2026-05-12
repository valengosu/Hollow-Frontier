#pragma once

#include "CoreMinimal.h"
#include "Components/Image.h"
#include "Engine/DataTable.h"
#include "RecipeConfig.generated.h"

USTRUCT(BlueprintType)
struct FRecipeConfig : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FName ItemName;
	
	UPROPERTY(EditAnywhere)
	FName ResourceItemName;
	
	UPROPERTY(EditAnywhere)
	int Count;
};