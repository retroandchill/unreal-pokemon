// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class PokemonAssets : ModuleRules
{
	public PokemonAssets(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
				// ... add public include paths required here ...
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				// ... add other private include paths required here ...
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"DeveloperSettings",
				"Paper2D", 
				"UnrealRanges",
				"RPGMenus"
				// ... add other public dependencies that you statically link with here ...
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				"PokemonCore",
				"PokemonData",
				"UnrealInjector", 
				"DynamicAssetLoader"
				// ... add private dependencies that you statically link with here ...	
			}
			);

		if (!string.IsNullOrEmpty(GetModuleDirectory("PaperZD")))
		{
			PublicDependencyModuleNames.Add("PaperZD");
			PublicDefinitions.Add("WITH_PAPERZD=1");
		}
		else
		{
			PublicDefinitions.Add("WITH_PAPERZD=0");
		}
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);
	}
}
