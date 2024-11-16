// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#pragma once
#include "Modules/ModuleManager.h"
#include "Toolkits/IToolkitHost.h"
#include "Toolkits/IToolkit.h"

//fwd declarations
class FPaperZDAnimBPEditor;
class FPaperZDAnimationSourceEditor;
struct FPaperZDGraphNodeFactory;
struct FPaperZDGraphPinFactory;
struct FPaperZDGraphPinConnectionFactory;

/**
 * Editor module for PaperZD
 */
class FPaperZDEditorModule : public IModuleInterface
{
	//Asset type actions
	TSharedPtr<class FAssetTypeActions_PaperZDAnimBP> AnimBPAssetTypeActions;
	TSharedPtr<class FAssetTypeActions_ZDAnimSeq> AnimSequenceAssetTypeActions;
	TSharedPtr<class FAssetTypeActions_ZDAnimationSource> AnimationSourceAssetTypeActions;

	//Graph Factory related
	TSharedPtr<FPaperZDGraphNodeFactory> GraphNodeFactory;
	TSharedPtr<FPaperZDGraphPinFactory> GraphPinFactory;
	TSharedPtr<FPaperZDGraphPinConnectionFactory> GraphPinConnectionFactory;

public:
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	//End IModuleInterface
	
	//Create and return a AnimBPEditor Instance
	TSharedRef<FPaperZDAnimBPEditor> CreateAnimBPEditor(const EToolkitMode::Type Mode, const TSharedPtr< class IToolkitHost >& InitToolkitHost, class UPaperZDAnimBP* InitAnimBP);

	//Create and return an editor for the given animation source, optionally requesting the given AnimSequence to be immediately selected
	TSharedRef<FPaperZDAnimationSourceEditor> CreateAnimationSourceEditor(const EToolkitMode::Type Mode, const TSharedPtr< class IToolkitHost >& InitToolkitHost, class UPaperZDAnimationSource* InAnimationSource, class UPaperZDAnimSequence* SelectedAnimSequence = nullptr);

	//Register Animation Blueprint Editor extensions
	void CustomizeAnimBlueprintEditorDetails(const TSharedRef<class IDetailsView>& InDetailsView);

private:
	//Registry
	void RegisterAssetActions();
	void UnregisterAssetActions();
	void RegisterCustomizations();
	void RegisterGraphFactories();
	void UnregisterGraphFactories();

	//Settings object setup
	void RegisterSettings();
	void UnregisterSettings();

	//For BlueprintCompilationManager
	void RegisterCompiler();
	static TSharedPtr<class FKismetCompilerContext> GetCompilerForAnimBP(class UBlueprint* BP, class FCompilerResultsLog& InMessageLog, const struct FKismetCompilerOptions& InCompileOptions);
};
