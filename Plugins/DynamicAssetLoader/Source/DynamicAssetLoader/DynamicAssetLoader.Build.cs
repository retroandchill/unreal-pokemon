// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class DynamicAssetLoader : ModuleRules
{
    public DynamicAssetLoader(ReadOnlyTargetRules Target)
        : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicIncludePaths.AddRange(
            new string[]
            {
                // ... add public include paths required here ...
            }
        );

        PrivateIncludePaths.AddRange(
            new string[]
            {
                // ... add other private include paths required here ...
            }
        );

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "RetroLib",
                "CoreUObject",
                "DeveloperSettings",
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
                // ... add private dependencies that you statically link with here ...
            }
        );

        DynamicallyLoadedModuleNames.AddRange(
            new string[]
            {
                // ... add any modules that your module loads dynamically here ...
            }
        );

        if (
            Target.bEnableCppCoroutinesForEvaluation
            && !string.IsNullOrEmpty(GetModuleDirectory("UE5Coro"))
        )
        {
            PublicDependencyModuleNames.Add("UE5Coro");
            PublicDefinitions.Add("DYN_ASSETS_WITH_UE5CORO=1");
        }
        else
        {
            PublicDefinitions.Add("DYN_ASSETS_WITH_UE5CORO=0");
        }

        bEnableExceptions = true;
    }
}
