using UnrealBuildTool;

public class DynamicAssetLoaderEditor : ModuleRules {
    public DynamicAssetLoaderEditor(ReadOnlyTargetRules Target) : base(Target) {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core" });

        PrivateDependencyModuleNames.AddRange(
            new string[] { "CoreUObject", "Engine", "Slate", "SlateCore", "DynamicAssetLoader", "RetroLibEditor" });
    }
}