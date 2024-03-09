using UnrealBuildTool;

public class PokemonTests : ModuleRules
{
    public PokemonTests(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore",
                "RPGMenus",
                "GridBased2D",
                "PokemonData",
                "PokemonCore",
                "PokemonUI",
                "CommonMath",
                "FunctionalTesting"
            }
        );
    }
}