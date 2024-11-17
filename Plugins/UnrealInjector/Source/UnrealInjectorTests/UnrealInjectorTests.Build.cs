using UnrealBuildTool;

public class UnrealInjectorTests : ModuleRules {
    public UnrealInjectorTests(ReadOnlyTargetRules Target) : base(Target) {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] {
            "Core",
        });

        PrivateDependencyModuleNames.AddRange(new string[] { "CoreUObject", "Engine", "Slate", "SlateCore",
                                                             "AutomationTestHelpers", "UnrealInjector",
                                                             "UnrealInjectorBlueprintNodes" });
    }
}