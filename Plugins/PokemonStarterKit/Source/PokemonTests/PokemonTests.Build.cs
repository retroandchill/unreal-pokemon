using UnrealBuildTool;

public class PokemonTests : ModuleRules
{
    public PokemonTests(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core", "PokemonBattle",
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore", 
                "AutomationTestHelpers",
                "PokemonData",
                "PokemonBlueprintNodes",
                "BlueprintGraph",
                "GraphEditor",
                "KismetCompiler",
                "UnrealEd",
                "PokemonEditorTools",
                "PokemonCore",
                "PokemonSettings",
                "UnrealInjector",
                "PokemonUtilities",
                "PokemonUI",
                "RPGMenus",
                "UMG",
                "GridBased2D", 
                "PokemonBattleUI",
                "GoogleTest", 
                "GameplayAbilities", 
                "GameplayTags", 
                "GameplayTasks",
                "CommonUI"
            }
        );
    }
}