using UnrealBuildTool;

public class DedicatedServers : ModuleRules
{
	public DedicatedServers(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"GameplayTags",
			"EnhancedInput",
			"HTTP",
			"GameLiftServerSDK",
			"NetCore",
			"MAPLE"
		});
		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"UMG",
			"Slate",
			"SlateCore",
			"Json",
			"JsonUtilities"
			
		});
	}
}
