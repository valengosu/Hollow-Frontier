#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Engine/Texture2D.h"
#include "Engine/StaticMesh.h"
#include "ItemConfig.generated.h"

USTRUCT(BlueprintType)
struct FItemConfig : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	int MaxPackCount;
	
	UPROPERTY(EditAnywhere)
	UTexture2D* Icon;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<AStaticMeshActor> StaticMeshActorClass;
};