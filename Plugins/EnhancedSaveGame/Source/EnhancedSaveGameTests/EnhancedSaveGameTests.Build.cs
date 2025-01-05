using UnrealBuildTool;

public class EnhancedSaveGameTests : ModuleRules {
    public EnhancedSaveGameTests(ReadOnlyTargetRules Target) : base(Target) {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "FunctionalTesting", "FunctionalTestingEditor",
                                                            "AutomationTestHelpers", "RetroLibTests" });

        PrivateDependencyModuleNames.AddRange(
            new string[] { "CoreUObject", "Engine", "Slate", "SlateCore", "EnhancedSaveGame" });
    }
}