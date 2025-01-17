using UnrealBuildTool;

public class PokemonMoves : ModuleRules
{
    public PokemonMoves(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "PokemonBattle",
                "RetroLib",
                "UE5Coro",
                "UE5CoroGAS",
                "GameplayAbilities",
                "PokemonData"
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore"
            }
        );
    }
}