#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GridWidget.h"
#include "Components/Overlay.h"
#include "Components/VerticalBox.h"
#include "Components/WrapBox.h"
#include "RecipeWidget.generated.h"

struct FRecipeConfig;

UCLASS()
class SURVIVAL_API URecipeWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	
	UFUNCTION()
	void ShowRecipeResourceWidgets(FName ItemName);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UGridWidget> GridWidgetClass;
	
	UPROPERTY(meta = (BindWidget))
	UWrapBox* WrapBox;
	
	UPROPERTY(meta = (BindWidget))
	UOverlay* CraftPanel;
	
	UPROPERTY(meta = (BindWidget))
	UVerticalBox* VerticalBox;
	
	UPROPERTY(meta = (BindWidget))
	UButton* CraftButton;
	
	UFUNCTION()
	void TryCraftItem();
	
private:
	FName CurrentCraftItemName;
};
