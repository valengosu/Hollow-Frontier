#include "ChangeAttributeCalculation.h"
#include "Attribute.h"
#include "GameplayTag.h"

void UChangeAttributeCalculation::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& Params,
	FGameplayEffectCustomExecutionOutput& Out) const
{
	Super::Execute_Implementation(Params, Out);
	
	const FGameplayEffectSpec& Spec = Params.GetOwningSpec();
	
	{
		float Value = Spec.GetSetByCallerMagnitude(TAG_Attribute_Hp, false, 0.f);		
		Out.AddOutputModifier(FGameplayModifierEvaluatedData(UAttribute::GetHpAttribute(), EGameplayModOp::Additive, Value));
	}
	
	{
		float Value = Spec.GetSetByCallerMagnitude(TAG_Attribute_Food, false, 0.f);		
		Out.AddOutputModifier(FGameplayModifierEvaluatedData(UAttribute::GetFoodAttribute(), EGameplayModOp::Additive, Value));
	}
	
	{
		float Value = Spec.GetSetByCallerMagnitude(TAG_Attribute_Water, false, 0.f);		
		Out.AddOutputModifier(FGameplayModifierEvaluatedData(UAttribute::GetWaterAttribute(), EGameplayModOp::Additive, Value));
	}
	
	{
		float Value = Spec.GetSetByCallerMagnitude(TAG_Attribute_Energy, false, 0.f);		
		Out.AddOutputModifier(FGameplayModifierEvaluatedData(UAttribute::GetEnergyAttribute(), EGameplayModOp::Additive, Value));
	}
}
