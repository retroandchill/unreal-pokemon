using UnrealBuildTool;

public class DynamicAssetLoaderNodes : ModuleRules
{
    public DynamicAssetLoaderNodes(ReadOnlyTargetRules Target) : base(Target)
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
                "DynamicAssetLoader",
                "UnrealEd",
                "BlueprintGraph",
                "KismetCompiler",
                "UnrealRanges", 
                "UnrealRangesBlueprintNodes",
                "ToolMenus"
            }
        );
    }
}