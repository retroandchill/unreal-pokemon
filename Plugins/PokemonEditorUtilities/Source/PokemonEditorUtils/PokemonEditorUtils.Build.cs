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
        
        if (Target.WithAutomationTests && !string.IsNullOrEmpty(GetModuleDirectory("AutomationTestHelpers")))
        {
            PrivateDependencyModuleNames.Add("AutomationTestHelpers");
            PrivateDefinitions.Add("HAS_AUTOMATION_HELPERS");
        }
    }
}