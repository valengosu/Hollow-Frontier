#include "AttributeComponent.h"
#include "AbilitySystemComponent.h"
#include "../Character/SurvivalCharacter.h"
#include "Attribute/Attribute.h"
#include "Attribute/GameplayTag.h"

UAttributeComponent::UAttributeComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UAttributeComponent::OnTimerTick()
{
	if (AbilitySystemComponent->HasMatchingGameplayTag(TAG_Character_Dead))
	{
		return;
	}
	
	//Attribute->ChangeAttribute(TAG_Attribute_Food, -1);
	Attribute->ChangeAttribute(TAG_Attribute_Water, -1);
	
	if (Attribute->GetFood() == 0 || Attribute->GetWater() == 0)
	{
		Attribute->ChangeAttribute(TAG_Attribute_Hp, -51);
	}
	
	if (Attribute->GetHp() == 0)
	{
		AbilitySystemComponent->AddLooseGameplayTag(TAG_Character_Dead);
		Cast<ASurvivalCharacter>(GetOwner())->Die();
	}
}

void UAttributeComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UAttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

