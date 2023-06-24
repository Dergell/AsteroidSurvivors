// Asteroid Survivors - Copyright (C) 2023 Tony Schmich

#include "AsteroidsAttributeSet.h"

#include "Net/UnrealNetwork.h"

void UAsteroidsAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UAsteroidsAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAsteroidsAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAsteroidsAttributeSet, DamageMultiplier, COND_None, REPNOTIFY_Always);
}

void UAsteroidsAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAsteroidsAttributeSet, Health, OldHealth);
}

void UAsteroidsAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAsteroidsAttributeSet, MaxHealth, OldMaxHealth);
}

void UAsteroidsAttributeSet::OnRep_DamageMultiplier(const FGameplayAttributeData& OldDamageMultiplier)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAsteroidsAttributeSet, DamageMultiplier, OldDamageMultiplier);
}