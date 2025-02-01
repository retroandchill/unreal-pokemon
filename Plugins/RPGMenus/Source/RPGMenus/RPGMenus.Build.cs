// "Unreal Pokémon" created by Retro & Chill.

using UnrealBuildTool;

public class RPGMenus : ModuleRules {
    public RPGMenus(ReadOnlyTargetRules Target) : base(Target) {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[] { "Core", "UMG", "InputCore", "CommonUI", "GameplayTags", "EnhancedInput",
                           "Paper2D", "PaperFlipbookWidget", "UnrealInjector", "RetroLib", "UE5Coro", "Simple2D" });

        PrivateDependencyModuleNames.AddRange(new string[] {
            "CoreUObject",
            "Engine",
            "Slate",
            "SlateCore",
            "DeveloperSettings",
            "CommonInput"
        });
    }
}