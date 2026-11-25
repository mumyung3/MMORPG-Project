// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MAPLEServerTarget : TargetRules
{
	public MAPLEServerTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Server;
		DefaultBuildSettings = BuildSettingsVersion.V4;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_3;
		ExtraModuleNames.Add("MAPLE");
		ExtraModuleNames.Add("DedicatedServers");
	}
}
