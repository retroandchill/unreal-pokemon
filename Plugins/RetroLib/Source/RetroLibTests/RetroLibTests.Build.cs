using UnrealBuildTool;

public class RetroLibTests : ModuleRules {
    public RetroLibTests(ReadOnlyTargetRules Target) : base(Target) {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "FunctionalTestingEditor", "RetroLib",
                                                            "FunctionalTesting", "FunctionalTestingEditor" });

        PrivateDependencyModuleNames.AddRange(
            new string[] { "CoreUObject", "Engine", "Slate", "SlateCore", "RetroLib", "GameplayTags" });

        if (Target.bEnableCppCoroutinesForEvaluation && !string.IsNullOrEmpty(GetModuleDirectory("UE5Coro"))) {
            PublicDependencyModuleNames.Add("UE5Coro");
        }
    }
}