// "Unreal Pokémon" created by Retro & Chill.

using UnrealBuildTool;

public class RPGMenus : ModuleRules
{
	public RPGMenus(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"UMG",
				"InputCore"
			}
		);

		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				"BlueprintGraph",
				"KismetCompiler",
				"UnrealEd",
				"CommonMath"
			}
		);
	}
}