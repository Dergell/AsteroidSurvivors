// Asteroid Survivors - Copyright (C) 2023 Tony Schmich

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

class UGameplayTagsManager;

/**
 * This singleton holds all native gameplay tags.
 */
struct FAsteroidsGameplayTags
{
public:
	static const FAsteroidsGameplayTags& Get() { return GameplayTags; }

	static void InitializeNativeTags();

	// Input Tags
	FGameplayTag InputTag_Move;
	FGameplayTag InputTag_Aim_Mouse;
	FGameplayTag InputTag_Aim_Stick;
	FGameplayTag InputTag_Attack;

	// States
	FGameplayTag State_Dead;
	FGameplayTag State_DamageMultiplied;

	// Effects
	FGameplayTag Effect_Cooldown;
	FGameplayTag Effect_Value;

	// Abilities
	FGameplayTag Ability_Attack;

	// Cues
	FGameplayTag GameplayCue_Effect_DamageMultiplier;
	FGameplayTag GameplayCue_Effect_Healing;
	FGameplayTag GameplayCue_Effect_Shield;

	// Cheats
	FGameplayTag Cheat_UnlimitedHealth;

protected:
	// Registers all of the tags with the GameplayTags Manager
	void AddAllTags(UGameplayTagsManager& Manager);

	// Helper function used by AddAllTags to register a single tag with the GameplayTags Manager
	void AddTag(FGameplayTag& OutTag, const ANSICHAR* TagName, const ANSICHAR* TagComment);

private:
	static FAsteroidsGameplayTags GameplayTags;
};