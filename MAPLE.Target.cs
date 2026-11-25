// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MAPLETarget : TargetRules
{
	public MAPLETarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V4;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_3;
		ExtraModuleNames.Add("MAPLE");
		ExtraModuleNames.Add("DedicatedServers");
	}
}
