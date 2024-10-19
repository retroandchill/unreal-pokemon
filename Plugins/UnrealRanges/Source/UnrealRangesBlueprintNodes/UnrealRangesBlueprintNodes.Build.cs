using UnrealBuildTool;

public class UnrealRangesBlueprintNodes : ModuleRules
{
    public UnrealRangesBlueprintNodes(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core", "UnrealRanges",
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
                "BlueprintGraph",
                "KismetCompiler",
                "UnrealEd",
                "ToolMenus",
            }
        );
    }
}