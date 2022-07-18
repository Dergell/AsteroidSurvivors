// Asteroid Survivors - Copyright (C) 2022 Tony Schmich

using UnrealBuildTool;
using System.Collections.Generic;

public class AsteroidsTarget : TargetRules
{
	public AsteroidsTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;

		ExtraModuleNames.AddRange( new string[] { "Asteroids" } );
    }
}
