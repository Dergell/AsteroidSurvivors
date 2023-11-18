// Asteroid Survivors - Copyright (C) 2023 Tony Schmich

using UnrealBuildTool;
using System.Collections.Generic;

public class AsteroidSurvivorsEditorTarget : TargetRules
{
	public AsteroidSurvivorsEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;

		ExtraModuleNames.AddRange( new string[] { "AsteroidSurvivors" } );
	}
}
