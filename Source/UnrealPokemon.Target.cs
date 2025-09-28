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
        RegisterModulesCreatedByRider();
    }

    private void RegisterModulesCreatedByRider()
    {
        ExtraModuleNames.AddRange(
            new string[]
            {
                "PokemonData",
                "GridBased2D",
                "InteractiveUI",
                "PokemonUI",
                "UnrealPokemon",
                "PaperZDHelpers",
                "PokemonBattle",
                "PokemonBattle",
            }
        );
    }
}
