using UnrealBuildTool;

public class UnrealRangesTests : ModuleRules
{
    public UnrealRangesTests(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core", 
                "FunctionalTestingEditor",
                "FunctionalTesting",
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore",
                "UnrealRanges",
                "AutomationTestHelpers"
            }
        );
    }
}