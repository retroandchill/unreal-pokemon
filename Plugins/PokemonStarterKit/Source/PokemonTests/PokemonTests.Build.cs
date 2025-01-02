using UnrealBuildTool;

public class PokemonTests : ModuleRules {
    public PokemonTests(ReadOnlyTargetRules Target) : base(Target) {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] {
            "Core",
            "PokemonBattle",
        });

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
                                                             "PokemonUtilities",
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
                                                             "Paper2D" });
    }
}