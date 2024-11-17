// "Unreal Pokémon" created by Retro & Chill.

using UnrealBuildTool;

public class RPGMenusEditor : ModuleRules {
    public RPGMenusEditor(ReadOnlyTargetRules Target) : base(Target) {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] {
            "Core",
            "RPGMenus",
        });

        PrivateDependencyModuleNames.AddRange(new string[] { "CoreUObject", "Engine", "Slate", "SlateCore", "UnrealEd",
                                                             "UnrealRangesEditor", "RPGMenus", "Paper2D" });
    }
}