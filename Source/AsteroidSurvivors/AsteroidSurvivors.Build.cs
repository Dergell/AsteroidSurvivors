// Asteroid Survivors - Copyright (C) 2023 Tony Schmich

using UnrealBuildTool;

public class AsteroidSurvivors : ModuleRules
{
	public AsteroidSurvivors(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicIncludePaths.AddRange(new string[]
		{
			"AsteroidSurvivors"
		});

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"ApplicationCore",
			"Engine",
			"PhysicsCore",
			"GameplayTags",
			"GameplayTasks",
			"GameplayAbilities",
			"AIModule",
			"Niagara"
		});

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"InputCore",
			"Slate",
			"SlateCore",
			"RenderCore",
			"EnhancedInput",
			"UMG"
		});

		// Generate compile errors if using DrawDebug functions in test/shipping builds.
		PublicDefinitions.Add("SHIPPING_DRAW_DEBUG_ERROR=1");
	}
}