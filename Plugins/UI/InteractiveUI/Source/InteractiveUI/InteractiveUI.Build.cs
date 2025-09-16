// "Unreal Pokémon" created by Retro & Chill.

using UnrealBuildTool;

public class InteractiveUI : ModuleRules
{
    public InteractiveUI(ReadOnlyTargetRules Target)
        : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "UMG",
                "InputCore",
                "CommonUI",
                "GameplayTags",
                "EnhancedInput",
                "Paper2D",
                "UnrealInjector",
                "RetroLib",
                "UE5Coro",
                "Simple2D", 
                "UnrealSharpCore", 
                "RangeV3",
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore",
                "DeveloperSettings",
                "CommonInput", 
                "CommonUtilities",
                "UnrealSharpBinds",
                "UnrealSharpAsync",
            }
        );
    }
}
