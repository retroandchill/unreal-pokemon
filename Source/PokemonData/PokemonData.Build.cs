using UnrealBuildTool;

public class PokemonData : ModuleRules
{
	public PokemonData(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		bUseRTTI = true;

		PublicDependencyModuleNames.AddRange(
			new[]
			{
				"Core", "Paper2D",
			}
		);

		PrivateDependencyModuleNames.AddRange(
			new[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				"BlueprintGraph",
				"KismetCompiler",
				"UnrealEd"
			}
		);
	}
}