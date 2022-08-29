// Asteroid Survivors - Copyright (C) 2022 Tony Schmich

using UnrealBuildTool;

public class Asteroids : ModuleRules
{
	public Asteroids(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" });

		PublicDependencyModuleNames.AddRange(new string[]
			{ "Niagara", "AIModule", "GameplayAbilities", "GameplayTags", "GameplayTasks" });

		PrivateDependencyModuleNames.AddRange(new string[] { });

		// Uncomment if you are using Slate UI
		PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}