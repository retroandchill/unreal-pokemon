using UnrealBuildTool;

public class PokemonBlueprintNodes : ModuleRules
{
    public PokemonBlueprintNodes(ReadOnlyTargetRules Target)
        : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[] { "Core", "PokemonData", "BlueprintGraph", "UnrealEd", "Simple2D" }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore",
                "KismetCompiler",
                "GraphEditor",
                "PokemonUI",
                "RPGMenus",
                "UMG",
                "ToolMenus",
            }
        );
    }
}
