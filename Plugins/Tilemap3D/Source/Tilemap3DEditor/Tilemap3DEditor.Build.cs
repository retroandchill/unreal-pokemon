using UnrealBuildTool;

public class Tilemap3DEditor : ModuleRules
{
    public Tilemap3DEditor(ReadOnlyTargetRules Target) : base(Target)
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
                "Blutility",
                "UMG",
                "CommonUI",
                "Tilemap3D", 
                "UnrealRanges",
                "InputCore"
            }
        );
    }
}