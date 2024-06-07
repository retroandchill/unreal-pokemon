using UnrealBuildTool;

public class PokemonUITests : ModuleRules
{
    public PokemonUITests(ReadOnlyTargetRules Target) : base(Target)
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
                "BlueprintGraph",
                "KismetCompiler",
                "UnrealEd",
                "ToolMenus",
                "PokemonCoreTests",
                "PokemonUI",
                "PokemonUIDeveloper",
                "UnrealInjector",
                "AutomationTestHelpers",
                "PokemonData",
                "UMG",
                "PokemonCore",
                "GridBased2D", "PokemonSettings", "PokemonTests"
            }
        );

        bUseRTTI = true;
    }
}