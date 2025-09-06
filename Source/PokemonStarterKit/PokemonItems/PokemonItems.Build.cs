using UnrealBuildTool;

public class PokemonItems : ModuleRules
{
    public PokemonItems(ReadOnlyTargetRules Target)
        : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[] { "Core", "PokemonCore", "PokemonAssets", "DynamicAssetLoader", "UE5Coro" }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[] { "CoreUObject", "Engine", "Slate", "SlateCore", "PokemonData" }
        );
    }
}
