#include "GE_ChangeAttribute.h"
#include "ChangeAttributeCalculation.h"

UGE_ChangeAttribute::UGE_ChangeAttribute()
{
	DurationPolicy = EGameplayEffectDurationType::Instant;
	
	FGameplayEffectExecutionDefinition ExecDef;
	ExecDef.CalculationClass = UChangeAttributeCalculation::StaticClass(); 
	Executions.Add(ExecDef);
}
