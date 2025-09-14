using UnrealBuildTool;

public class Simple2DEditor : ModuleRules
{
    public Simple2DEditor(ReadOnlyTargetRules Target)
        : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "Simple2D",
                "UnrealEd",
                "EditorWidgets",
                "RetroLib",
                "KismetWidgets",
                "DeveloperSettings",
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore",
                "Paper2D",
                "Paper2DEditor",
                "PropertyEditor",
                "EditorInteractiveToolsFramework",
                "InputCore",
                "RenderCore",
                "Projects",
            }
        );
    }
}
