// "Unreal Pokémon" created by Retro & Chill.

using UnrealBuildTool;

public class UnrealPokemonTarget : TargetRules
{
    public UnrealPokemonTarget(TargetInfo Target)
        : base(Target)
    {
        Type = TargetType.Game;
        DefaultBuildSettings = BuildSettingsVersion.V4;
        IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_6;
        bEnableCppCoroutinesForEvaluation = true;
        RegisterModulesCreatedByRider();
    }

    private void RegisterModulesCreatedByRider()
    {
        ExtraModuleNames.AddRange(
            new string[]
            {
                "PokemonData",
                "PokemonCore",
                "GridBased2D",
                "RPGMenus",
                "PokemonUI",
                "UnrealPokemon",
                "TileReplacer",
                "PaperZDHelpers",
            }
        );
    }
}
