// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MAPLEClientTarget : TargetRules
{
	public MAPLEClientTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Client;
		DefaultBuildSettings = BuildSettingsVersion.V4;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_3;
		ExtraModuleNames.Add("MAPLE");
		ExtraModuleNames.Add("DedicatedServers");
	}
}
