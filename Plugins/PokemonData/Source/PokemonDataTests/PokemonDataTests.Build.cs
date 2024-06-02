using UnrealBuildTool;

public class PokemonDataTests : ModuleRules
{
    public PokemonDataTests(ReadOnlyTargetRules Target) : base(Target)
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
                "PokemonData",
                "PokemonDataDeveloper",
                "AutomationTestHelpers",
                "DeveloperSettings",
                "GraphEditor",
                "BlueprintGraph",
                "KismetCompiler",
                "UnrealEd"
            }
        );
        
        bUseRTTI = true;
    }
}