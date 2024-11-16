// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "EditorUndoClient.h"
#include "UObject/GCObject.h"
#include "Misc/NotifyHook.h"
#include "WorkflowOrientedApp/WorkflowCentricApplication.h"
#include "Editors/Util/PaperZDEditorDelegates.h"

class UPaperZDAnimationSource;
class UPaperZDAnimSequence;
class SPaperZDAnimAssetBrowser;
class FPaperZDAnimationSourceViewportClient;
class SWidget;

namespace PaperZDAnimationSourceModes
{
	// Mode identifiers
	extern const FName AnimationMode;
}

namespace PaperZDAnimationSourceTabs
{
	// Tab identifiers
	extern const FName SourceDetailsTab;
	extern const FName AssetBrowserTab;
	extern const FName AssetDetailsTab;
	extern const FName ViewportTab;
	extern const FName DocumentTab;
	extern const FName DetailsViewTab;
	extern const FName SourceNotifiesTab;
}

/**
 * Editor object used for Animation Sources and their sequences.
 */
class FPaperZDAnimationSourceEditor : public FWorkflowCentricApplication//, public FGCObject //, public FEditorUndoClient, public FNotifyHook
{
	/* The animation source we're currently editing. */
	TObjectPtr<UPaperZDAnimationSource> AnimSource;

	/* Selected animation asset we're inspecting. */
	TWeakObjectPtr<UPaperZDAnimSequence> SelectedAnimAsset;

	/* Asset data of the selected animation, that we use to distinguish in case it gets deleted while open. */
	FAssetData AnimAssetData;

	/* The animation document currently being edited */
	TWeakPtr<SDockTab> SharedAnimDocumentTab;

	//Widgets we need to reference later, created through the tab factories.
	TWeakPtr<SPaperZDAnimAssetBrowser> AssetBrowserPtr;
	TWeakPtr<FPaperZDAnimationSourceViewportClient> ViewportClientPtr;

	/** Details panel */
	TSharedPtr<class IDetailsView> DetailsView;

public:
	FPaperZDAnimationSourceEditor();
	~FPaperZDAnimationSourceEditor();

	//getters
	FORCEINLINE UPaperZDAnimationSource* GetAnimationSourceBeingEdited() const { return AnimSource; }
	FORCEINLINE TWeakPtr<FPaperZDAnimationSourceViewportClient> GetViewportClient() const { return ViewportClientPtr; }

	/* Handle required for the detail inspector delegates. */
	UObject* HandleGetAnimationSourceAsset();
	UObject* HandleGetSelectedAnimationAsset();

	/* Handles the selection of notify and other sub-objects that need to be shown on their own details view. */
	void HandleObjectsSelected(const TArray<UObject*>& InObjects);

	/* Called when an asset gets deleted, to handle any AnimSequence removal. */
	void HandleAssetRemoved(const FAssetData& RemovedAssetData);

	//~ Begin IToolkit Interface
	virtual void RegisterTabSpawners(const TSharedRef<class FTabManager>& TabManager) override;
	virtual void UnregisterTabSpawners(const TSharedRef<class FTabManager>& TabManager) override;
	virtual FName GetToolkitFName() const override;
	virtual FText GetBaseToolkitName() const override;
	virtual FString GetWorldCentricTabPrefix() const override;
	virtual FLinearColor GetWorldCentricTabColorScale() const override;
	//~ End IToolkit Interface

	//Init the animation source editor
	void InitAnimationSourceEditor(const EToolkitMode::Type Mode, const TSharedPtr<IToolkitHost>& InitToolkitHost, UPaperZDAnimationSource* InAnimationSource);

	/* Called to let the editor know that the given asset wants to be opened. */
	void OpenAsset(const FAssetData& AssetData);

	/* Called when the asset browser gets created by the tab factories. */
	void HandleAssetBrowserCreated(const TSharedRef<SPaperZDAnimAssetBrowser>& AssetBrowser);

	/* Called when the viewport gets created by the tab factories. */
	void HandleViewportCreated(const TSharedRef<SPaperZDAnimationSourceViewport>& AnimationViewport);

	/* Called when the details view gets created. */
	void HandleDetailsCreated(const TSharedRef<class IDetailsView>& InDetailsView);

private:
	/* Extends the toolbar, so we can add the "mode switch" widget on it. */
	void ExtendToolbar();

	/* Called whenever the animation source changes its properties. */
	void OnAnimationSourcePropertyChange();

	/* Called when an internal widget requests a tab to be opened */
	void InvokeTab(const FTabId& TabId);

	/* Creates the corresponding widget for the given animation object. */
	TSharedRef<SWidget> CreateEditorWidgetForAnimAsset(UPaperZDAnimSequence* AnimAsset);
};
