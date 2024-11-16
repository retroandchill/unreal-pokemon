// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#pragma once
#include "CoreMinimal.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/Views/SListView.h"
#include "EditorUndoClient.h"

/** Delegate fired when a notify is selected */
DECLARE_DELEGATE_OneParam(FOnItemSelectedSignature, const FName& /*InSelectedNotify*/);

//Fwd declarations
class UPaperZDAnimationSource;
class FAssetEditorToolkit;
class SInlineEditableTextBlock;
class SSearchBox;
struct FNotificationInfo;

class FPaperZDAnimNotifyDisplayInfo
{
public:
	FName Name;

	/** Handle to editable text block for rename */
	TSharedPtr<SInlineEditableTextBlock> InlineEditableText;

	/** Flag to say whether this is a new item we are creating */
	bool bIsNew;

	/** Static function for creating a new item, but ensures that you can only have a TSharedRef to one */
	static TSharedRef<FPaperZDAnimNotifyDisplayInfo> Make(const FName& NotifyName)
	{
		return MakeShareable(new FPaperZDAnimNotifyDisplayInfo(NotifyName));
	}

protected:
	/** Hidden constructor, always use Make above */
	FPaperZDAnimNotifyDisplayInfo(const FName& InNotifyName)
		: Name(InNotifyName)
		, bIsNew(false)
	{}

	/** Hidden constructor, always use Make above */
	FPaperZDAnimNotifyDisplayInfo() {}
};

/** Widgets list type */
typedef SListView<TSharedPtr<FPaperZDAnimNotifyDisplayInfo>> SPaperZDAnimNotifyListType;

/**
 * Displays and gives access to managing the existing custom notifies on a given AnimationSource
 */
class SPaperZDAnimationSourceNotifies : public SCompoundWidget, public FEditorUndoClient//, public FGCObject
{
public:
	SLATE_BEGIN_ARGS(SPaperZDAnimationSourceNotifies)
		: _IsPicker(false)
	{}

	/** Delegate fired when a notify is selected */
	SLATE_EVENT(FOnItemSelectedSignature, OnItemSelected)

	/** Whether we should use this dialog as a picker or an editor. In picker mode we cant add, remove or rename notifies. */
	SLATE_ARGUMENT(bool, IsPicker)

	SLATE_END_ARGS()

	/* Constructs this widget. */
	void Construct(const FArguments& InArgs, UPaperZDAnimationSource* InAnimSource, const TSharedPtr<FAssetEditorToolkit>& InHostingApp = nullptr);
	~SPaperZDAnimationSourceNotifies();

	/** FEditorUndoClient interface */
	virtual void PostUndo(bool bSuccess) override;
	virtual void PostRedo(bool bSuccess) override;

// 	// FGCObject interface start
// 	virtual void AddReferencedObjects( FReferenceCollector& Collector ) override;
// 	// FGCObject interface end

	/** When user attempts to commit the name of a track*/
	bool OnVerifyNotifyNameCommit( const FText& NewName, FText& OutErrorMessage, TSharedPtr<FPaperZDAnimNotifyDisplayInfo> Item );

	/** When user commits the name of a track*/
	void OnNotifyNameCommitted( const FText& NewName, ETextCommit::Type, TSharedPtr<FPaperZDAnimNotifyDisplayInfo> Item );

	/** Dummy handler to stop editable text boxes swallowing our list selected events */
	bool IsSelected() { return false; }

private:
	/** Called when the user changes the contents of the search box */
	void OnFilterTextChanged( const FText& SearchText );

	/** Called when the user changes the contents of the search box */
	void OnFilterTextCommitted( const FText& SearchText, ETextCommit::Type CommitInfo );

	/** Delegate handler for generating rows in NotifiesListView */ 
	TSharedRef<ITableRow> GenerateNotifyRow( TSharedPtr<FPaperZDAnimNotifyDisplayInfo> InInfo, const TSharedRef<STableViewBase>& OwnerTable );

	/** Delegate handler called when the user right clicks in NotifiesListView */
	TSharedPtr<SWidget> OnGetContextMenuContent() const;

	/** Delegate handler for when the user selects something in NotifiesListView */
	void OnNotifySelectionChanged( TSharedPtr<FPaperZDAnimNotifyDisplayInfo> Selection, ESelectInfo::Type SelectInfo );

	/** Delegate handler for determining whether we can show the delete menu options */
	bool CanPerformDelete() const;

	/** Delegate handler for deleting anim notifies */
	void OnDeleteAnimNotify();

	/* Deletes an existing notify and updates every animation that currently includes them. */
	int32 DeleteExistingNotifies(TArray<FName> NotifyNames);

	/** Delegate handler for determining whether we can show the rename menu options */
	bool CanPerformRename() const;

	/** Delegate handler for adding anim notifies */
	void OnAddAnimNotify();

	/** Delegate handler for renaming anim notifies */
	void OnRenameAnimNotify();

	/* Renames an existing notify and updates every animation that currently includes them. */
	int32 RenameExistingNotify(FName NewNotifyName, FName ExistingNotify);

	/** Wrapper that populates NotifiesListView using current filter test */
	void RefreshNotifiesListWithFilter();

	/** Populates NotifiesListView based on the skeletons notifies and the supplied filter text */
	void CreateNotifiesList( const FString& SearchText = FString("") );

	/** Utility function to display notifications to the user */
	void NotifyUser( FNotificationInfo& NotificationInfo );

	/** Handler function for when notifies are modified on the skeleton */
	void OnNotifiesChanged();

	/** Handle when an item is scrolled into view, triggers a rename for new items */
	void OnItemScrolledIntoView(TSharedPtr<FPaperZDAnimNotifyDisplayInfo> InItem, const TSharedPtr<ITableRow>& InTableRow);
// 
// 	/** Handle find references from the context menu */
// 	void OnFindReferences();

	/* Obtains the AnimSequences that are bound to the given animation source. */
	void GetCompatibleAnimSequences(TArray<struct FAssetData>& OutAssets) const;

	/* Obtains the Animation Blueprints that inherit from this animation source. */
	void GetCompatibleAnimBPs(TArray<struct FAssetData>& OutAssets) const;

	/** The animation source we are currently editing */
	TObjectPtr<UPaperZDAnimationSource> AnimationSource;

	/** SSearchBox to filter the notify list */
	TSharedPtr<SSearchBox>	NameFilterBox;

	/** Widget used to display the list of notifies */
	TSharedPtr<SPaperZDAnimNotifyListType> NotifiesListView;

	/** A list of notifies. Used by the NotifiesListView. */
	TArray<TSharedPtr<FPaperZDAnimNotifyDisplayInfo>> NotifyList;

	/** Current text typed into NameFilterBox */
	FText FilterText;

	/** Delegate fired when a notify is selected */
	FOnItemSelectedSignature OnItemSelected;

	/** The app we are embedded in */
	TWeakPtr<class FAssetEditorToolkit> WeakHostingApp;

	/** Whether we should use this dialog as a picker or an editor. In picker mode we cant add, remove or rename notifies. */
	bool bIsPicker;
};