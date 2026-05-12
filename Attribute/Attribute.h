// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "Attribute.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class SURVIVAL_API UAttribute : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	UAttribute();
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
	void ChangeAttribute(FGameplayTag Tag, int Value);
	
	UPROPERTY(BlueprintReadOnly)
	FGameplayAttributeData Hp;
	ATTRIBUTE_ACCESSORS(ThisClass, Hp);
	
	UPROPERTY(BlueprintReadOnly)
	FGameplayAttributeData Food;
	ATTRIBUTE_ACCESSORS(ThisClass, Food);
	
	UPROPERTY(BlueprintReadOnly)
	FGameplayAttributeData Water;
	ATTRIBUTE_ACCESSORS(ThisClass, Water);
	
	UPROPERTY(BlueprintReadOnly)
	FGameplayAttributeData Energy;
	ATTRIBUTE_ACCESSORS(ThisClass, Energy);
	
	static int MaxValue;
};
