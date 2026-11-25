// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MAPLE : ModuleRules
{
    public MAPLE(ReadOnlyTargetRules Target) : base(Target)
    {
        PrivateDependencyModuleNames.AddRange(new string[] { "AugmentedReality" });
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new[]
        {
            "Core",
            "CoreUObject",
            "Engine",
            "InputCore",

            "Json",
            "JsonUtilities",
            "HTTP",

            "UMG",
            "Slate",
            "SlateCore",
            "EnhancedInput",
            "GameplayTags",
            "GameplayTasks",

            "AnimGraphRuntime",
            "MotionWarping",
            "Niagara",
            "NavigationSystem",
            "AIModule", 
            "GameLiftServerSDK",
            "DedicatedServers"
        });
    }
}
