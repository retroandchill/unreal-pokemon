using UnrealBuildTool;

public class PokemonUtilitiesTests : ModuleRules
{
    public PokemonUtilitiesTests(ReadOnlyTargetRules Target) : base(Target)
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
                "PokemonUtilities",
                "AutomationTestHelpers"
            }
        );

        bUseRTTI = true;
    }
}