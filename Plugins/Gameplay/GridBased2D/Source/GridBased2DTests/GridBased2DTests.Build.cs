using UnrealBuildTool;

public class GridBased2DTests : ModuleRules {
    public GridBased2DTests(ReadOnlyTargetRules Target) : base(Target) {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] {
            "Core",
        });

        PrivateDependencyModuleNames.AddRange(new string[] { "CoreUObject", "Engine", "Slate", "SlateCore",
                                                             "GridBased2D", "AutomationTestHelpers",
                                                             "FunctionalTesting" });
    }
}