﻿using UnrealBuildTool;

public class PokemonBattleUI : ModuleRules {
    public PokemonBattleUI(ReadOnlyTargetRules Target) : base(Target) {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[] { "Core", "RPGMenus", "PokemonBattle", "PokemonUI", "PokemonData", "PokemonCore", "CommonUI",
                           "DynamicAssetLoader", "UnrealInjector", "RetroLib", "UE5Coro", "Simple2D" });

        PrivateDependencyModuleNames.AddRange(new string[] {
            "CoreUObject",
            "Engine",
            "Slate",
            "SlateCore",
            "UMG",
            "PokemonAssets",
            "GameplayAbilities",
            "GameplayTags",
            "Paper2D",
        });
    }
}