using UnrealBuildTool;

public class PokemonEncounters : ModuleRules
{
    public PokemonEncounters(ReadOnlyTargetRules Target)
        : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[] { "Core", "GameplayTags", "GameplayAbilities" }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore",
                "PokemonData",
                "PokemonCore",
            }
        );
    }
}
