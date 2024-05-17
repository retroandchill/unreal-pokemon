using UnrealBuildTool;

public class PokemonAssetsEditor : ModuleRules
{
    public PokemonAssetsEditor(ReadOnlyTargetRules Target) : base(Target)
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
                "PokemonAssets",
                "EditorSubsystem",
                "DeveloperSettings"
            }
        );
    }
}