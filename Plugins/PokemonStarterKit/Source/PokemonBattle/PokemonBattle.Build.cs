﻿using UnrealBuildTool;

public class PokemonBattle : ModuleRules
{
    public PokemonBattle(ReadOnlyTargetRules Target) : base(Target)
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
                "PokemonUtilities",
                "DeveloperSettings",
                "DynamicAssetLoader", "PokemonAssets"
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
                "Paper2D"
            }
        );
    }
}