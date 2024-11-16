// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

using UnrealBuildTool;
using System.IO;

public class PaperZDEditor : ModuleRules
{
	public PaperZDEditor(ReadOnlyTargetRules TargetRules) : base(TargetRules)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        PublicIncludePaths.AddRange(
			new string[] {
                Path.Combine(ModuleDirectory, "Public"),
				//"PaperZDEditor/Public",
				// ... add public include paths required here ...
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
                Path.Combine(ModuleDirectory, "Private"),
                Path.Combine(ModuleDirectory, "Private/Factories"),
                Path.Combine(ModuleDirectory, "Private/AssetTypeActions"),
                Path.Combine(ModuleDirectory, "Private/Editors"),
                Path.Combine(ModuleDirectory, "Private/Editors/Slate"),
                Path.Combine(ModuleDirectory, "Private/Editors/Util"),
                Path.Combine(ModuleDirectory, "Private/Editors/DetailCustomizations"),
                Path.Combine(ModuleDirectory, "Private/Compilers"),
                Path.Combine(ModuleDirectory, "Private/Graphs"),
                Path.Combine(ModuleDirectory, "Private/Graphs/Nodes"),
                Path.Combine(ModuleDirectory, "Private/Graphs/Nodes/Slate"),
				/*"PaperZDEditor/Private",
                "PaperZDEditor/Private/Factories",
                "PaperZDEditor/Private/AssetTypeActions",
                "PaperZDEditor/Private/Editors",
                "PaperZDEditor/Private/Editors/Slate",
                "PaperZDEditor/Private/Editors/Util",
                "PaperZDEditor/Private/Editors/DetailCustomizations",
                "PaperZDEditor/Private/Compilers",
                "PaperZDEditor/Private/Graphs",
                "PaperZDEditor/Private/Graphs/Nodes",
                "PaperZDEditor/Private/Graphs/Nodes/Slate",*/
				// ... add other private include paths required here ...
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
                "Paper2D",
                //Check which are needed
                "UnrealEd",
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
                //Check which are needed
                "PropertyEditor",
                "LevelEditor",
                "UnrealEd",
                "AssetTools",
                "EditorStyle",
                "GraphEditor",
                "Kismet",
                "KismetCompiler",
                "BlueprintGraph", //REMOVE? USED BY TRANSITION GRAPH
                "KismetWidgets",
                "ClassViewer",
                "Settings",
                "InputCore",
                "ToolMenus",
                "TimeManagement",
                "Sequencer",
                "MovieScene",
                "MovieSceneTools",
                "SequencerWidgets",
                "EditorWidgets",
                "ApplicationCore",
                "SubobjectDataInterface",

                //Plugin Management
                "Projects",
                

                //ENDCHECK
                "PaperZD"
				// ... add private dependencies that you statically link with here ...	
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);

#if UE_5_1_OR_LATER
        PrivateDefinitions.Add("ZD_VERSION_INLINED_CPP_SUPPORT=1");
#else
		PrivateDefinitions.Add("ZD_VERSION_INLINED_CPP_SUPPORT=0");
#endif
    }
}
