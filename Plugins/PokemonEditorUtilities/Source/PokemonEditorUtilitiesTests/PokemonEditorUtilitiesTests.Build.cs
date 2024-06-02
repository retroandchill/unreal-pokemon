using UnrealBuildTool;

public class PokemonEditorUtilitiesTests : ModuleRules
{
    public PokemonEditorUtilitiesTests(ReadOnlyTargetRules Target) : base(Target)
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
                "PokemonEditorUtils",
                "AutomationTestHelpers",
                "PokemonData",
                "UnrealEd",
                "DesktopPlatform"
            }
        );

        bUseRTTI = true;
    }
}