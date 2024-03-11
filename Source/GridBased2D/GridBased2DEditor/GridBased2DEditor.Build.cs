// "Unreal Pokémon" created by Retro & Chill.
using UnrealBuildTool;

public class GridBased2DEditor : ModuleRules
{
    public GridBased2DEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core", "GridBased2D"
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore",
                "UnrealEd",
                "Paper2DEditor"
            }
        );
    }
}