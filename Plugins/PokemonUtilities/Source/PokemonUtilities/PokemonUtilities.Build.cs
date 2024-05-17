// "Unreal Pokémon" created by Retro & Chill.

using UnrealBuildTool;

public class PokemonUtilities : ModuleRules
{
	public PokemonUtilities(ReadOnlyTargetRules Target) : base(Target)
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
				"SlateCore"
			}
		);
		
		if (Target.WithAutomationTests && !string.IsNullOrEmpty(GetModuleDirectory("AutomationTestHelpers")))
		{
			PrivateDependencyModuleNames.Add("AutomationTestHelpers");
			PrivateDependencyModuleNames.Add("UnrealEd");
			PrivateDefinitions.Add("HAS_AUTOMATION_HELPERS");
		}
	}
}