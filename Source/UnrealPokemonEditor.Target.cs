// "Unreal Pokémon" created by Retro & Chill.

using System.Collections.Generic;
using System.Linq;
using UnrealBuildTool;

public class UnrealPokemonEditorTarget : TargetRules
{
	public UnrealPokemonEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		bUseUnityBuild = false;
		DefaultBuildSettings = BuildSettingsVersion.V4;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_4;
		RegisterModulesCreatedByRider();
	}

	private void RegisterModulesCreatedByRider()
	{
		ExtraModuleNames.AddRange(new string[]
		{
			"PokemonData", "PokemonEditorUtils", "PokemonUtilities", "PokemonCore", "GridBased2D", "GridBased2DEditor",
			"RPGMenus", "PokemonUI", "RPGMenusEditor", "CommonMath", "PokemonTests", "PokemonUIDeveloper",
			"PokemonDataDeveloper", "RPGMenusDeveloper", "UnrealPokemon", "TileReplacer"
		});
	}
}