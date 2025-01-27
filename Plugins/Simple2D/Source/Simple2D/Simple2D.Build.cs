// Copyright Epic Games, Inc. All Rights Reserved.

using System.IO;
using UnrealBuildTool;

public class Simple2D : ModuleRules {
    public Simple2D(ReadOnlyTargetRules Target) : base(Target) {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "Paper2D", "RetroLib" });

        PrivateDependencyModuleNames.AddRange(new string[] {
            "CoreUObject", "Engine", "Slate", "SlateCore", "Engine", "RenderCore", "Renderer", "RHI"
            // ... add private dependencies that you statically link with here ...
        });

        DynamicallyLoadedModuleNames.AddRange(new string[] {
            // ... add any modules that your module loads dynamically here ...
        });
    }
}
