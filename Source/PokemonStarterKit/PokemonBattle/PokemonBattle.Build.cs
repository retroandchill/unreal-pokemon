using UnrealBuildTool;

public class PokemonBattle : ModuleRules
{
    public PokemonBattle(ReadOnlyTargetRules Target)
        : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "PokemonData",
                "PokemonCore",
                "GameplayAbilities",
                "GameplayTags",
                "GameplayTasks",
                "DeveloperSettings",
                "PokemonAssets",
                "UE5Coro",
                "UE5CoroGAS",
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore",
                "UnrealInjector",
                "PokemonAssets",
                "Paper2D",
                "RetroLib",
                "Simple2D",
            }
        );

        if (!string.IsNullOrEmpty(GetModuleDirectory("PaperZD")))
        {
            PublicDependencyModuleNames.Add("PaperZD");
        }
    }
}
