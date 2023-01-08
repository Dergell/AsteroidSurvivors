// Asteroid Survivors - Copyright (C) 2022 Tony Schmich


#include "AsteroidsGameplayTags.h"
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

	AddTag(State_Dead, "State.Dead", "State Dead.");

	AddTag(Effect_Cooldown, "Effect.Cooldown", "Cooldown effect.");
	AddTag(Effect_Damage, "Effect.Damage", "Damage effect.");

	AddTag(Ability_Attack, "Ability.Attack", "Attack ability.");

	AddTag(Cheat_UnlimitedHealth, "Cheat.UnlimitedHealth", "God mode.");
}

void FAsteroidsGameplayTags::AddTag(FGameplayTag& OutTag, const ANSICHAR* TagName, const ANSICHAR* TagComment)
{
	OutTag = UGameplayTagsManager::Get().AddNativeGameplayTag(FName(TagName), FString(TEXT("(Native) ")) + FString(TagComment));
}