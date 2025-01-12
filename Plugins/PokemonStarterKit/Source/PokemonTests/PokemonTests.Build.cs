using UnrealBuildTool;

public class PokemonTests : ModuleRules {
    public PokemonTests(ReadOnlyTargetRules Target) : base(Target) {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "PokemonBattle", "FunctionalTesting" });

        PrivateDependencyModuleNames.AddRange(new string[] { "CoreUObject",
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
                                                             "UnrealInjector",
                                                             "PokemonUI",
                                                             "RPGMenus",
                                                             "UMG",
                                                             "PokemonBattleUI",
                                                             "GoogleTest",
                                                             "GameplayAbilities",
                                                             "GameplayTags",
                                                             "GameplayTasks",
                                                             "CommonUI",
                                                             "CommonGame",
                                                             "DeveloperSettings",
                                                             "EnhancedInput",
                                                             "PokemonEncounters",
                                                             "ToolWidgets",
                                                             "RetroLib",
                                                             "RetroLibBlueprintNodes",
                                                             "PokemonAssets",

                                                             "ToolMenus",
                                                             "DynamicAssetLoaderNodes",
                                                             "AutomationTest",
                                                             "Paper2D",
                                                             "RetroLibTests",
                                                             "RPGMenusTests",
                                                             "UE5Coro",
                                                             "UE5CoroGAS"
        });
    }
}