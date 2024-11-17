﻿using UnrealBuildTool;

public class PokemonOverworld : ModuleRules {
    public PokemonOverworld(ReadOnlyTargetRules Target) : base(Target) {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] {
            "Core",
            "GameplayAbilities",
        });

        PrivateDependencyModuleNames.AddRange(new string[] { "CoreUObject", "Engine", "Slate", "SlateCore",
                                                             "PokemonEncounters", "PokemonUtilities", "GameplayTasks",
                                                             "GameplayTags" });
    }
}