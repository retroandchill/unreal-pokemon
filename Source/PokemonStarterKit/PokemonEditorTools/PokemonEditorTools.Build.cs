using UnrealBuildTool;

public class PokemonEditorTools : ModuleRules
{
    public PokemonEditorTools(ReadOnlyTargetRules Target)
        : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "PokemonData", "DeveloperSettings" });

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore",
                "DeveloperSettings",
                "EditorSubsystem",
                "InputCore",
                "UnrealEd",
                "GraphEditor",
                "BlueprintGraph",
                "ToolWidgets",
                "GameplayTagsEditor",
                "GameplayTags",
                "UnrealSharpCore",
                "StructUtilsEditor",
            }
        );
    }
}
