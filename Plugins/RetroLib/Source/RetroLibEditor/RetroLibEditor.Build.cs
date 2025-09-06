using UnrealBuildTool;

public class RetroLibEditor : ModuleRules
{
    public RetroLibEditor(ReadOnlyTargetRules Target)
        : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[] { "Core", "RetroLib", "EditorWidgets", "PropertyEditor" }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[] { "CoreUObject", "Engine", "Slate", "SlateCore" }
        );
    }
}
