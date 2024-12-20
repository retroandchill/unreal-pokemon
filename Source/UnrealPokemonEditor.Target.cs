// "Unreal Pokémon" created by Retro & Chill.

using System.Collections.Generic;
using System.Linq;
using UnrealBuildTool;

public class UnrealPokemonEditorTarget : TargetRules {
    public UnrealPokemonEditorTarget(TargetInfo Target) : base(Target) {
        Type = TargetType.Editor;
        DefaultBuildSettings = BuildSettingsVersion.V4;
        IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_5;
        // This does nothing now, but once https://github.com/EpicGames/UnrealEngine/pull/12561 is merged, then this
        // will automatically enable code coverage on Clang-CL builds.
        bCodeCoverage = true;
        RegisterModulesCreatedByRider();
    }

    private void RegisterModulesCreatedByRider() {
        ExtraModuleNames.AddRange(new string[] { "PokemonData", "PokemonUtilities", "PokemonCore", "GridBased2D",
                                                 "RPGMenus", "PokemonUI", "RPGMenusEditor", "CommonMath",
                                                 "UnrealPokemon", "TileReplacer", "UnrealPokemonEditor",
                                                 "PaperZDHelpers" });
    }
}