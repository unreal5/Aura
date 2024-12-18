// 版本没有，拷贝自由。

using UnrealBuildTool;

public class Aura : ModuleRules
{
	public Aura(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "GameplayAbilities"
		});

		PrivateDependencyModuleNames.AddRange(new string[]
			{ "GameplayTasks", "GameplayTags", "NavigationSystem", "Niagara", "AIModule" });

		PrivateIncludePaths.AddRange(new string[] { "Aura/" });
	}
}