// "Unreal Pokémon" created by Retro & Chill.

using UnrealBuildTool;

public class RPGMenus : ModuleRules
{
	public RPGMenus(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"UMG",
				"InputCore", 
				"CommonUI", 
				"UnrealRanges"
			}
		);

		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				"CommonMath",
				"Paper2D",
				"DeveloperSettings"
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