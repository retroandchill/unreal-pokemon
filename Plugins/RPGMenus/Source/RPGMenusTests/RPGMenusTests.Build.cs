using UnrealBuildTool;

public class RPGMenusTests : ModuleRules
{
    public RPGMenusTests(ReadOnlyTargetRules Target) : base(Target)
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
                "RPGMenus",
                "RPGMenusDeveloper",
                "RPGMenusEditor",
                "AutomationTestHelpers",
                "UMG",
                "InputCore",
                "UnrealEd",
                "BlueprintGraph",
                "KismetCompiler",
                "CommonUI"
            }
        );
    }
}