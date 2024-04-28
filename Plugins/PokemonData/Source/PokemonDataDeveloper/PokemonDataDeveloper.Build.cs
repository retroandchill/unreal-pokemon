using UnrealBuildTool;

public class PokemonDataDeveloper : ModuleRules
{
	public PokemonDataDeveloper(ReadOnlyTargetRules Target) : base(Target)
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
				"GraphEditor",
				"BlueprintGraph",
				"KismetCompiler",
				"UnrealEd"
			}
		);
		
		if (Target.WithAutomationTests && !string.IsNullOrEmpty(GetModuleDirectory("AutomationTestHelpers")))
		{
			PrivateDependencyModuleNames.Add("AutomationTestHelpers");
			PrivateDefinitions.Add("HAS_AUTOMATION_HELPERS");
		}
	}
}