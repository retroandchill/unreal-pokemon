using UnrealBuildTool;

public class PokemonUIDeveloper : ModuleRules
{
    public PokemonUIDeveloper(ReadOnlyTargetRules Target) : base(Target)
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
                "PokemonUI",
                "RPGMenus",
                "BlueprintGraph",
                "KismetCompiler",
                "UnrealEd",
                "ToolMenus"
            }
        );
    }
}