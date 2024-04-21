// "Unreal Pokémon" created by Retro & Chill.
using UnrealBuildTool;

public class PokemonEditorUtils : ModuleRules
{
    public PokemonEditorUtils(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "PokemonData"
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
                "DesktopPlatform"
            }
        );
    }
}