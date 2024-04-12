using UnrealBuildTool;

public class PokemonTests : ModuleRules
{
	public PokemonTests(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		bUseRTTI = true;

		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core", "FunctionalTesting", "RPGMenus"
			}
		);

		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				"BlueprintGraph",
				"KismetCompiler",
				"UnrealEd",
				"GraphEditor",
				"RPGMenus",
				"GridBased2D",
				"PokemonData",
				"PokemonCore",
				"PokemonUI",
				"CommonMath",
				"FunctionalTesting",
				"AutomationDriver",
				"UMG",
				"InputCore", 
				"PokemonEditorUtils", 
				"PokemonDataDeveloper", "RPGMenusDeveloper", "RPGMenusEditor", "GridBased2DEditor"
			}
		);
	}
}