// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "InventoryComponent.generated.h"

struct FItem
{
	FName RowName;
	int Count;
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnInventoryChanged, const TArray<FItem>&);

UCLASS()
class SURVIVAL_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;
	
	void AddItem(FName RowName, int Count);
	bool RemoveItem(FName RowName, int Count);
	int GetItemCount(FName RowName); 
	bool CraftItem(FName RowName);
	
	TArray<FItem> Items;
};
