// "Unreal Pokémon" created by Retro & Chill.

using UnrealBuildTool;

public class PokemonUI : ModuleRules
{
	public PokemonUI(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"PokemonCore",
				"RPGMenus",
				"GridBased2D"
			}
		);

		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				"UMG",
				"DeveloperSettings"
			}
		);
	}
}