﻿// "Unreal Pokémon" created by Retro & Chill.

using UnrealBuildTool;

public class GridBased2D : ModuleRules
{
	public GridBased2D(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core", "Paper2D"
			}
		);

		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				"EnhancedInput",
				"CommonMath",
				"TileReplacer",
				"DeveloperSettings"
			}
		);
		
		if (Target.WithAutomationTests && !string.IsNullOrEmpty(GetModuleDirectory("AutomationTestHelpers")))
		{
			PrivateDependencyModuleNames.Add("AutomationTestHelpers");
			PrivateDefinitions.Add("HAS_AUTOMATION_HELPERS");
		}
	}
}