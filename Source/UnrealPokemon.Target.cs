// "Unreal Pokémon" created by Retro & Chill.

using UnrealBuildTool;

public class UnrealPokemonTarget : TargetRules
{
	public UnrealPokemonTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V4;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_4;
		RegisterModulesCreatedByRider();
	}

	private void RegisterModulesCreatedByRider()
	{
		ExtraModuleNames.AddRange(new string[]
		{
			"PokemonData", "PokemonUtilities", "PokemonCore", "GridBased2D",
			"RPGMenus", "PokemonUI", "CommonMath", "UnrealPokemon", "TileReplacer", "PaperZDHelpers"
		});
	}
}