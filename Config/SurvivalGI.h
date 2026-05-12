#pragma once

#include "CoreMinimal.h"
#include "WaterBodyOceanActor.h"
#include "Hash/BuzHash.h"
#include "SurvivalGI.generated.h"

class AABuildPreviewActor;
class USurvivalHUDWidget;
struct FRecipeConfig;


UCLASS(Blueprintable)
class SURVIVAL_API USurvivalGI : public UGameInstance
{
	GENERATED_BODY()
	
public:
	virtual void Init() override;
	void GetRecipeResourceItems(FName ItemName, TArray<FName> &ResourceItemNames, TArray<int> &ResourceItemCounts);
	AWaterBodyOcean* GetOcean();
	
public:
	UPROPERTY()
	AWaterBodyOcean* Ocean;
	
	UPROPERTY(EditAnywhere)
	TArray<UDataTable*> AllTables;
	
	UPROPERTY(EditAnywhere)
	TMap<const UScriptStruct*, UDataTable*> TableStructs;	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<USurvivalHUDWidget> StatsWidgetClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AABuildPreviewActor> BuildPreviewActorClass;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Anim")
	TObjectPtr<UAnimMontage> SkillMontage;
	
	TArray<FRecipeConfig*> RecipeConfigRows;
	TSet<FName> RecipeItem;	
};

template<typename T>
T* GetTableRow(UObject* WorldContext, FName RowName)
{
	USurvivalGI* GI = Cast<USurvivalGI>(WorldContext->GetWorld()->GetGameInstance());
	UDataTable* Table = GI->TableStructs.FindRef(T::StaticStruct());
	if (Table == nullptr)
		return nullptr;
	
	return Table->FindRow<T>(RowName, TEXT(""));
}