// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#pragma once
#include "CoreMinimal.h"
#include "UObject/GCObject.h"
#include "Toolkits/IToolkitHost.h"
#include "Toolkits/AssetEditorToolkit.h"
#include "BlueprintEditor.h"

class UPaperZDAnimBP;

/**
 * Editor for the Animation blueprint object.
 */
class FPaperZDAnimBPEditor : public FBlueprintEditor
{
	/* Currently edited animation blueprint. */
	TObjectPtr<UPaperZDAnimBP> AnimBPBeingEdited;

public:
	FPaperZDAnimBPEditor();
	~FPaperZDAnimBPEditor();

	//~Begin FBlueprintEditor Interface
	virtual void DeleteSelectedNodes() override;
	virtual FGraphAppearanceInfo GetGraphAppearance(class UEdGraph* InGraph) const override;
	//virtual void HandleSetObjectBeingDebugged(UObject* InObject) override;
	//~End FBlueprintEditor Interface

	//FAssetEditorToolkit
	virtual FName GetToolkitFName() const override;				
	virtual FText GetBaseToolkitName() const override;			
	virtual FText GetToolkitName() const override;
	virtual FText GetToolkitToolTipText() const override;
	virtual FString GetWorldCentricTabPrefix() const override;	
	virtual FLinearColor GetWorldCentricTabColorScale() const override;
	//End FAssetEditorToolkit

	// FSerializableObject interface
	virtual void AddReferencedObjects(FReferenceCollector& Collector) override;
	// End of FSerializableObject interface

	/* Initializes the AnimBP Editor and enters the default AnimBP Blueprint Tab */
	void InitAnimBPEditor(const EToolkitMode::Type Mode, const TSharedPtr<class IToolkitHost>& InitToolkitHost, class UPaperZDAnimBP* InitAnimBP);

	/* Adds the ChangeMode toolbar to the normal blueprint Toolbar */
	void ExtendToolbar();

	/* Registers OnTick/OnInit... on the AnimBP */
	static void RegisterDefaultEventNodes();

	/* Unregisters OnTick/OnInit... on the AnimBP */
	static void UnregisterDefaultEventNodes();
};
