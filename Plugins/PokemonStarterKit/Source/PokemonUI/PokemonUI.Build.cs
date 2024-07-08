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
				"PokemonData",
				"PokemonCore",
				"RPGMenus",
				"GridBased2D",
				"PokemonAssets",
				"CommonUI"
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
				"RenderCore",
				"UnrealInjector", 
				"PokemonSettings"
			}
		);
	}
}