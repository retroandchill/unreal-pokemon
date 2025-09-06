using UnrealBuildTool;

public class GameDataAccessToolsEditor : ModuleRules
{
    public GameDataAccessToolsEditor(ReadOnlyTargetRules target)
        : base(target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            ["Core", "UnrealSharpCore", "UnrealEd", "GameDataAccessTools"]
        );

        PrivateDependencyModuleNames.AddRange(
            [
                "CoreUObject",
                "UnrealSharpBinds",
                "Engine",
                "Slate",
                "SlateCore",
                "WorkspaceMenuStructure",
                "InputCore",
                "UnrealSharpProcHelper",
                "JsonUtilities",
                "PropertyPath",
                "ToolWidgets",
                "GameplayTags",
                "GameplayTagsEditor",
                "Json",
                "JsonUtilities",
            ]
        );
    }
}
