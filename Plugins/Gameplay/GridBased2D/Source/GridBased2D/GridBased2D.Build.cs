// "Unreal Pokémon" created by Retro & Chill.

using UnrealBuildTool;

public class GridBased2D : ModuleRules
{
    public GridBased2D(ReadOnlyTargetRules Target)
        : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[] { "Core", "Paper2D" }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore",
                "EnhancedInput",
                "TileReplacer",
                "DeveloperSettings",
                "AudioPlaybackManager", "RangeV3", "CommonUtilities",
            }
        );
    }
}
