using UnrealBuildTool;

public class UnrealInjectorEditor : ModuleRules {
    public UnrealInjectorEditor(ReadOnlyTargetRules Target) : base(Target) {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] {
            "Core",
        });

        PrivateDependencyModuleNames.AddRange(new string[] { "CoreUObject", "Engine", "Slate", "SlateCore",
                                                             "UnrealInjector", "PropertyEditor", "UnrealRanges",
                                                             "UnrealEd" });

        if (Target.WithAutomationTests && !string.IsNullOrEmpty(GetModuleDirectory("AutomationTestHelpers"))) {
            PrivateDependencyModuleNames.Add("AutomationTestHelpers");
            PrivateDefinitions.Add("HAS_AUTOMATION_HELPERS");
        }
    }
}