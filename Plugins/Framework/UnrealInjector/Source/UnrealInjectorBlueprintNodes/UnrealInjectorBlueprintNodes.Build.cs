using UnrealBuildTool;

public class UnrealInjectorBlueprintNodes : ModuleRules {
    public UnrealInjectorBlueprintNodes(ReadOnlyTargetRules Target) : base(Target) {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] {
            "Core",
            "UnrealInjector",
        });

        PrivateDependencyModuleNames.AddRange(new string[] { "CoreUObject", "Engine", "Slate", "SlateCore",
                                                             "KismetCompiler", "BlueprintGraph", "UnrealEd" });
    }
}