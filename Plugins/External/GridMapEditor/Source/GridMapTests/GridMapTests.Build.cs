using UnrealBuildTool;

public class GridMapTests : ModuleRules
{
    public GridMapTests(ReadOnlyTargetRules Target) : base(Target)
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
                "GridMapEditor",
                "GridMap",
                "UnrealRanges",
                "InteractiveToolsFramework",
                "UnrealEd",
                "EditorFramework"
            }
        );
    }
}