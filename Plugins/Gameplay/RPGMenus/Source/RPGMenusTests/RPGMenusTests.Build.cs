using UnrealBuildTool;

public class RPGMenusTests : ModuleRules
{
    public RPGMenusTests(ReadOnlyTargetRules Target)
        : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "RPGMenus" });

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore",
                "RPGMenus",
                "AutomationTestHelpers",
                "UMG",
                "InputCore",
                "UnrealEd",
                "BlueprintGraph",
                "KismetCompiler",
                "CommonUI",
                "GameplayTags",
                "DynamicAssetLoader",
                "Paper2D",
                "Simple2D",
            }
        );
    }
}
