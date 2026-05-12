#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Engine/Texture2D.h"
#include "Engine/StaticMesh.h"
#include "BuildPartConfig.generated.h"

class ABuildPartActor;

USTRUCT(BlueprintType)
struct FBuildPartConfig : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	UStaticMesh* Mesh;

	UPROPERTY(EditAnywhere)
	TEnumAsByte<ETraceTypeQuery> SocketChannel;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<ABuildPartActor> BuildPartActorClass;
};