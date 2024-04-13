// "Unreal Pokémon" created by Retro & Chill.

using System.Collections.Generic;
using System.Linq;
using UnrealBuildTool;

public class UnrealPokemonEditorTarget : TargetRules
{
	public UnrealPokemonEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V5;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_4;
		WindowsPlatform.Compiler = WindowsCompiler.Clang;
		bOverrideBuildEnvironment = true;
		AdditionalCompilerArguments += "-fprofile-instr-generate -fcoverage-mapping";
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