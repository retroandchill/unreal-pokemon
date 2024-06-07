// "Unreal Pokémon" created by Retro & Chill.

using UnrealBuildTool;

public class PokemonData : ModuleRules
{
	public PokemonData(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(
			new[]
			{
				"Core", "Paper2D", "PokemonUtilities"
			}
		);

		PrivateDependencyModuleNames.AddRange(
			new[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				"DeveloperSettings", "PokemonSettings"
			}
		);
	}
}