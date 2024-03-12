// "Unreal Pokémon" created by Retro & Chill.

using UnrealBuildTool;

public class PokemonCore : ModuleRules
{
	public PokemonCore(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(
			new[]
			{
				"Core", "PokemonData",
			}
		);

		PrivateDependencyModuleNames.AddRange(
			new[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				"DeveloperSettings",
				"CommonMath"
			}
		);
	}
}