#include "Attribute.h"
#include "GameplayTag.h"
#include "GE_ChangeAttribute.h"
#include "GameplayEffectExtension.h"
#include "AttributeSet.h"

int UAttribute::MaxValue = 100;

UAttribute::UAttribute()
{
	Hp.SetBaseValue(MaxValue);
	Hp.SetCurrentValue(MaxValue);

	Food.SetBaseValue(MaxValue);
	Food.SetCurrentValue(MaxValue);

	Water.SetBaseValue(MaxValue);
	Water.SetCurrentValue(MaxValue);

	Energy.SetBaseValue(MaxValue);
	Energy.SetCurrentValue(MaxValue);
}

void UAttribute::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data); 

	if (Data.EvaluatedData.Attribute == GetHpAttribute())
	{
		SetHp(FMath::Clamp(GetHp(), 0.f, UAttribute::MaxValue));
	}
	
	if (Data.EvaluatedData.Attribute == GetFoodAttribute())
	{
		SetFood(FMath::Clamp(GetFood(), 0.f, UAttribute::MaxValue));
	}

	if (Data.EvaluatedData.Attribute == GetWaterAttribute())
	{
		SetWater(FMath::Clamp(GetWater(), 0.f, UAttribute::MaxValue));
	}

	if (Data.EvaluatedData.Attribute == GetEnergyAttribute())
	{
		SetEnergy(FMath::Clamp(GetEnergy(), 0.f, UAttribute::MaxValue));
	}
}

void UAttribute::ChangeAttribute(FGameplayTag Tag, int Value)
{
	FGameplayEffectSpecHandle Spec = GetOwningAbilitySystemComponent()->MakeOutgoingSpec(UGE_ChangeAttribute::StaticClass(), 1.f, GetOwningAbilitySystemComponent()->MakeEffectContext());
	Spec.Data->SetSetByCallerMagnitude(Tag, Value);
	GetOwningAbilitySystemComponent()->ApplyGameplayEffectSpecToSelf(*Spec.Data.Get());
}


