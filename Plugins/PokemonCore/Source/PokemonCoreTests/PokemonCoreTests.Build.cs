using UnrealBuildTool;

public class PokemonCoreTests : ModuleRules
{
    public PokemonCoreTests(ReadOnlyTargetRules Target) : base(Target)
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
                "PokemonCore",
                "UnrealInjector",
                "AutomationTestHelpers",
                "PokemonData", "PokemonSettings"
            }
        );
        
        bUseRTTI = true;
    }
}