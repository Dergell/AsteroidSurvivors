// Asteroid Survivors - Copyright (C) 2022 Tony Schmich

using UnrealBuildTool;
using System.Collections.Generic;

public class AsteroidsEditorTarget : TargetRules
{
	public AsteroidsEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;

		ExtraModuleNames.AddRange( new string[] { "Asteroids", "AsteroidsEditor" } );
	}
}
