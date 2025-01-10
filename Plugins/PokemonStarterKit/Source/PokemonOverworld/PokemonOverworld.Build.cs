using UnrealBuildTool;

public class PokemonOverworld : ModuleRules {
    public PokemonOverworld(ReadOnlyTargetRules Target) : base(Target) {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] {
            "Core",
            "GameplayAbilities",
            "PokemonCore"
        });

        PrivateDependencyModuleNames.AddRange(new string[] { "CoreUObject", "Engine", "Slate", "SlateCore",
                                                             "PokemonEncounters", "GameplayTasks",
                                                             "GameplayTags" });
    }
}