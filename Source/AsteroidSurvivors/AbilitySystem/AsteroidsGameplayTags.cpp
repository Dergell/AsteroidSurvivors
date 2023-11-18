// Asteroid Survivors - Copyright (C) 2023 Tony Schmich

#include "AbilitySystem/AsteroidsGameplayTags.h"
#include "GameplayTagsManager.h"
#include "Engine/EngineTypes.h"

FAsteroidsGameplayTags FAsteroidsGameplayTags::GameplayTags;

void FAsteroidsGameplayTags::InitializeNativeTags()
{
	UGameplayTagsManager& GameplayTagsManager = UGameplayTagsManager::Get();

	GameplayTags.AddAllTags(GameplayTagsManager);

	GameplayTagsManager.DoneAddingNativeTags();
}

void FAsteroidsGameplayTags::AddAllTags(UGameplayTagsManager& Manager)
{
	AddTag(InputTag_Move, "InputTag.Move", "Move input.");
	AddTag(InputTag_Aim_Mouse, "InputTag.Aim.Mouse", "Aim (mouse) input.");
	AddTag(InputTag_Aim_Stick, "InputTag.Aim.Stick", "Aim (stick) input.");
	AddTag(InputTag_Attack, "InputTag.Attack", "Attack input.");
	AddTag(InputTag_Pause, "InputTag.Pause", "Pause input.");

	AddTag(State_Dead, "State.Dead", "State Dead.");
	AddTag(State_DamageMultiplied, "State.DamageMultiplied", "State DamageMultiplied.");

	AddTag(Effect_Cooldown, "Effect.Cooldown", "Cooldown of the effect.");
	AddTag(Effect_Value, "Effect.Value", "Value of the effect.");

	AddTag(Ability_Attack, "Ability.Attack", "Attack ability.");
	
	AddTag(GameplayCue_Effect_DamageMultiplier, "GameplayCue.Effect.DamageMultiplier", "GameplayCue Effect DamageMultiplier.");
	AddTag(GameplayCue_Effect_Healing, "GameplayCue.Effect.Healing", "GameplayCue Effect Healing.");
	AddTag(GameplayCue_Effect_Shield, "GameplayCue.Effect.Shield", "GameplayCue Effect Shield.");

	AddTag(Cheat_UnlimitedHealth, "Cheat.UnlimitedHealth", "God mode.");
}

void FAsteroidsGameplayTags::AddTag(FGameplayTag& OutTag, const ANSICHAR* TagName, const ANSICHAR* TagComment)
{
	OutTag = UGameplayTagsManager::Get().AddNativeGameplayTag(FName(TagName), FString(TEXT("(Native) ")) + FString(TagComment));
}