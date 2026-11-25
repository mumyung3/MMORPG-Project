// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MAPLEEditorTarget : TargetRules
{
	public MAPLEEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V4;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_3;
		ExtraModuleNames.Add("MAPLE");
		ExtraModuleNames.Add("DedicatedServers");
	}
}
