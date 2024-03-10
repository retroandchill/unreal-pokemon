using UnrealBuildTool;

public class RPGMenusDeveloper : ModuleRules
{
    public RPGMenusDeveloper(ReadOnlyTargetRules Target) : base(Target)
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
                "BlueprintGraph",
                "KismetCompiler",
                "UnrealEd",
            }
        );
    }
}