// Asteroid Survivors - Copyright (C) 2023 Tony Schmich

using UnrealBuildTool;
using System.Collections.Generic;

public class AsteroidSurvivorsTarget : TargetRules
{
	public AsteroidSurvivorsTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;

		ExtraModuleNames.AddRange( new string[] { "AsteroidSurvivors" } );
    }
}
