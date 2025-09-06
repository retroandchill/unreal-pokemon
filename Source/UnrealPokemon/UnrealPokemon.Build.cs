// "Unreal Pokémon" created by Retro & Chill.
using UnrealBuildTool;

public class UnrealPokemon : ModuleRules
{
    public UnrealPokemon(ReadOnlyTargetRules Target)
        : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[] { "Core", "CoreUObject", "Engine", "InputCore" }
        );

        PrivateDependencyModuleNames.AddRange(new string[] { "PaperZD" });

        // Uncomment if you are using Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled
        // attribute set to true
    }
}
