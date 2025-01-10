// "Unreal Pokémon" created by Retro & Chill.

using UnrealBuildTool;

public class PokemonUI : ModuleRules {
    public PokemonUI(ReadOnlyTargetRules Target) : base(Target) {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "PokemonData", "PokemonCore", "RPGMenus",
                                                            "PokemonAssets", "CommonUI", "CommonGame", "GameplayTags",
                                                            "EnhancedInput", "DeveloperSettings", "PokemonItems",
                                                            "PaperFlipbookWidget", "UE5Coro", "RetroLib" });

        PrivateDependencyModuleNames.AddRange(new string[] {
            "CoreUObject",
            "Engine",
            "Slate",
            "SlateCore",
            "UMG",
            "RenderCore",
            "UnrealInjector",
            "CommonInput",
            "PokemonItems",
            "Paper2D",
            "DynamicAssetLoader",
            "EnhancedSaveGame",
        });
    }
}