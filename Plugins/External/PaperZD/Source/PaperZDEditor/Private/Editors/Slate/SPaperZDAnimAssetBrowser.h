// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#pragma once
#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Editor/ContentBrowser/Public/ContentBrowserDelegates.h"
#include "Editors/Util/PaperZDEditorDelegates.h"
#include "AssetRegistry/ARFilter.h"

class FPaperZDAnimationSourceEditor;
class SMenuAnchor;

/**
 * An animation asset browser related to an animation source.
 */
class SPaperZDAnimAssetBrowser : public SCompoundWidget
{
	/* Source editor hosting app. */
	TWeakPtr<FPaperZDAnimationSourceEditor> SourceEditorPtr;

	/* Pointer to the asset picker. */
	TSharedPtr<SWidget> AssetPickerPtr;

	/* Array of assets that were opened, in order. */
	TArray<FAssetData> AssetHistory;

	/* Current index in history. */
	int32 CurrentHistoryIdx;

	/* Delegate to sync the asset picker to selected assets. */
	FSyncToAssetsDelegate SyncToAssetsDelegate;

	/* Delegate for getting the current selection of the picker. */
	FGetCurrentSelectionDelegate GetCurrentSelectionDelegate;

	/* Delegate for requesting a refresh on the asset view. */
	FRefreshAssetViewDelegate RefreshAssetViewDelegate;

	/* Delegate used to set the AR filter after the fact */
	FSetARFilterDelegate SetFilterDelegate;

	/* Asset registry filter we are using for querying, stored so we can modify it. */
	FARFilter Filter;

	/* Set of tags to prevent creating details view columns for (infrequently used). */
	TSet<FName> AssetRegistryTagsToIgnore;

	/* Commands handled by the widget. */
	TSharedPtr<FUICommandList> Commands;

	/* Delegate called when we request an asset to be opened. */
	FPaperZDEditorDelegates::OnOpenAssetSignature OnOpenAsset;

	/* True if we requested a new asset to be created last frame. */
	bool bCreatedAssetLastFrame;

	/* True if there's a deferred rename on the selected asset to be executed. */
	bool bPendingRenameSelection;

public:
	SLATE_BEGIN_ARGS(SPaperZDAnimAssetBrowser)
	{}
	SLATE_ARGUMENT(FPaperZDEditorDelegates::OnOpenAssetSignature, OnOpenAsset)
	SLATE_END_ARGS()

	/* Constructs the widget. */
	void Construct(const FArguments& InArgs, TSharedPtr<FPaperZDAnimationSourceEditor> InSourceEditor);

	/* Used for processing the command's hotkeys. */
	virtual FReply OnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent) override;

	/* Used to detect rename requests. */
	virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;

	/* Opens the given animation asset. */
	void SelectAsset(const FAssetData& Asset);

private:
	/* Called to construct the menu widget for the "Add New Animation" button. */
	TSharedRef<SWidget> CreateAddNewMenuWidget();

	/* Builds the internal dropdown menu from AddNew */
	void BuildAddNewMenu(FMenuBuilder& MenuBuilder);

	/* Creates the context menu for the asset selection. */
	TSharedPtr<SWidget> OnGetAssetContextMenu(const TArray<FAssetData>& SelectedAssets);

	/* Creates the history menu when right clicking either the back or forward buttons on history. */
	TSharedRef<SWidget> CreateHistoryMenu(bool bInBackHistory) const;

	/* Goes back in history. */
	FReply OnGoBackInHistory();

	/* True if the user can go backwards in history. */
	bool CanStepBackInHistory() const;

	/* Goes forward in history. */
	FReply OnGoForwardInHistory();

	/* True if the user can go forward in history. */
	bool CanStepForwardInHistory() const;

	/* Called when clicking on the history menu, to check if a menu should pop out. */
	FReply OnMouseDownHistory(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent, TWeakPtr< SMenuAnchor > InMenuAnchor);

	/* Clears the whole history set. */
	void ClearHistory();

	/* Clears the history set from the current index and forward. */
	void ClearForwardHistory();

	/* Forces the asset being pointed by the history index to be opened and set as the current one. */
	void SetAssetHistoryIndex(int32 Index);

	/* Opens the given asset, either from history or not. */
	void OnRequestOpenAsset(const FAssetData& AssetData, bool bFromHistory);

	/* Checks if the given column should be shown on the asset picker. */
	bool CanShowColumnForAssetRegistryTag(FName AssetType, FName TagName) const;

	/* Called when the user requests the given asset to be duplicated. */
	void OnRenameSelection();

	/* True if the option to duplicate the selected asset is enabled. */
	bool CanRenameSelection() const;

	/* Called when the user requests the given asset to be duplicated. */
	void OnDuplicateSelection();

	/* True if the option to duplicate the selected asset is enabled. */
	bool CanDuplicateSelection() const;

	/* Called when the user requests the given asset to be duplicated. */
	void OnDeleteSelection();

	/* True if the option to duplicate the selected asset is enabled. */
	bool CanDeleteSelection() const;

	/* Called when the user requests the given asset to be highlighted on the content browser. */
	void FindInContentBrowser();

	/* True if the option to find in content browser is enabled (single selection). */
	bool CanFindInContentBrowser() const;

	/* Delegate to handle "Save" context menu option */
	void SaveSelectedAssets() const;

	/* Delegate to handle enabling the "Save" context menu option */
	bool CanSaveSelectedAssets() const;

	/* Obtains the list of the packages that are currently selected due to the assets being highlighted on the browser. */
	void GetSelectedPackages(const TArray<FAssetData>& Assets, TArray<UPackage*>& OutPackages) const;

	/* Handles the request for creation of a new AnimSequence. */
	void HandleCreateAnimSequence();
};
