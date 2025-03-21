// 版权没有，拷贝自由。

using UnrealBuildTool;

public class Aura : ModuleRules
{
	public Aura(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		PublicDependencyModuleNames.AddRange(new string[]
			{ "Core", "CoreUObject", "Engine", "InputCore", "GameplayAbilities", "GameplayTasks", "GameplayTags" });
		PrivateDependencyModuleNames.AddRange(new string[]
			{ "EnhancedInput", "UMG", "Slate", "SlateCore", "AIModule", "NavigationSystem"});
	}
}