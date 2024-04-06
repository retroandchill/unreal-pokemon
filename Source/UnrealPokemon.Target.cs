// "Unreal Pokémon" created by Retro & Chill.

using UnrealBuildTool;

public class UnrealPokemonTarget : TargetRules
{
	public UnrealPokemonTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V4;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_4;
		WindowsPlatform.Compiler = WindowsCompiler.Clang;
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