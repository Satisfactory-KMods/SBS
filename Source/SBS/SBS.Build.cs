using UnrealBuildTool;

public class SBS : ModuleRules
{
	public SBS(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new[]
		{
			"Core", "CoreUObject",
			"Engine",
			"InputCore",
			"OnlineSubsystem", "OnlineSubsystemUtils", "OnlineSubsystemNULL",
			"SignificanceManager",
			"PhysX", "APEX", "PhysXVehicles", "ApexDestruction",
			"AkAudio",
			"ReplicationGraph",
			"HTTP", "RHI", "UMG",
			"AIModule",
			"NavigationSystem",
			"AssetRegistry",
			"GameplayTasks",
			"AnimGraphRuntime",
			"Slate", "SlateCore",
			"Json", "KBFL", "RenderCore", "KPrivateCodeLib"
		});


		if (Target.Type == TargetRules.TargetType.Editor)
			PublicDependencyModuleNames.AddRange(new[] { "OnlineBlueprintSupport", "AnimGraph" });
		PublicDependencyModuleNames.AddRange(new[] { "FactoryGame", "SML", "KBFL" });
		PrivateDependencyModuleNames.AddRange(
			new[]
			{
				"Core", "CoreUObject", "Engine", "Serialization", "InputCore",
				"SML",
				"FactoryGame", "KPrivateCodeLib", "KBFL", "AbstractInstance"
				// ... add private dependencies that you statically link with here ...	
			}
		);
	}
}