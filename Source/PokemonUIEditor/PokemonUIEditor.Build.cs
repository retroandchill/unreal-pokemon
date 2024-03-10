using UnrealBuildTool;

public class PokemonUIEditor : ModuleRules
{
    public PokemonUIEditor(ReadOnlyTargetRules Target) : base(Target)
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
                "UnrealEd",
                "BlueprintGraph",
                "KismetCompiler",
                "ToolMenus"
            }
        );
    }
}