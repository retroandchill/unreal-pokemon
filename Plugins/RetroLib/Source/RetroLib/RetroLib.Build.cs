// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class RetroLib : ModuleRules
{
	public RetroLib(ReadOnlyTargetRules Target) : base(Target)
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
				"Engine"
				// ... add other public dependencies that you statically link with here ...
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore"
				// ... add private dependencies that you statically link with here ...	
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);

		PublicDefinitions.Add(Target.bEnableCppModules ? "RETROLIB_WITH_MODULES=1" : "RETROLIB_WITH_MODULES=0");
		PublicDefinitions.Add(Target.bEnableCppCoroutinesForEvaluation ? "RETROLIB_WITH_COROUTINES=1" : "RETROLIB_WITH_COROUTINES=0");
		
		
	}
}
