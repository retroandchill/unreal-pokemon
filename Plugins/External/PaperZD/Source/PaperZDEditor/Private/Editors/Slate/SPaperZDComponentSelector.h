// Copyright 2017 ~ 2023 Critical Failure Studio Ltd. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Templates/SubclassOf.h"
#include "UObject/WeakObjectPtrTemplates.h"
#include "Editors/Slate/SPaperZDComponentTreeView.h"
#include "Misc/TextFilter.h"

class AActor;
class UActorComponent;
class SSearchBox;
class SHeaderRow;

/**
 * A specialized selector that searches through the given actor and displays the compatible components as a picker view.
 */
class SPaperZDComponentSelector final : public SCompoundWidget
{
public:
	/* Delegate called when the user selects a valid component. */
	DECLARE_DELEGATE_TwoParams(FOnComponentSelectedSignature, const UActorComponent*, const FName);
	DECLARE_DELEGATE(FOnClearedSelectionSignature);

private:
	SLATE_BEGIN_ARGS(SPaperZDComponentSelector)
		: _OnComponentSelected()
		, _AllowClear(false)
		{}

		SLATE_EVENT(FOnComponentSelectedSignature, OnComponentSelected)
		SLATE_EVENT(FOnClearedSelectionSignature, OnClearedSelection)
		SLATE_ATTRIBUTE(TArray<TSubclassOf<UActorComponent>>, AllowedClassesFilter)
		SLATE_ATTRIBUTE(TArray<TSubclassOf<UActorComponent>>, DisallowedClassesFilter)
		SLATE_ARGUMENT(bool, AllowClear)
	
	SLATE_END_ARGS()

	/* Constructs this widget. */
	void Construct(const FArguments& InArgs, AActor* InActorContext);

	/* Rebuilds and refreshes the tree view. */
	void RebuildTreeView();

	/** Called by STreeView to generate a table row for the specified item */
	TSharedRef<ITableRow> OnGenerateRowForComponentTree(FPaperZDComponentTreeItemPtr Item, const TSharedRef<STableViewBase>& OwnerTable);

	/** Called by STreeView to get child items for the specified parent item */
	void OnGetChildrenForComponentTree(FPaperZDComponentTreeItemPtr InParent, TArray<FPaperZDComponentTreeItemPtr>& OutChildren);

	/* Needed for the text filter to work. */
	void PopulateSearchStrings(const FPaperZDComponentTreeItem& Item, TArray<FString>& OutSearchStrings) const;

	/* Called by the editable text control when the filter text is changed by the user */
	void OnFilterTextChanged(const FText& InFilterText);

	/* Obtain the current header sort mode. */
	EColumnSortMode::Type GetHeaderSortMode() const;

	/* Called when sorting the columns. */
	void OnColumnSortModeChanged(const EColumnSortPriority::Type SortPriority, const FName& ColumnId, const EColumnSortMode::Type InSortMode);

	/* Called by STreeView when the tree's selection has changed */
	void OnTreeSelectionChanged(FPaperZDComponentTreeItemPtr TreeItem, ESelectInfo::Type SelectInfo);

	/* Sorts the tree with the stored sort mode. */
	void SortTreeDataSource();

	/* Clears the selection. */
	void OnClear();

	/* Checks if the component can be cleared or not. */
	bool CanClear() const;

private:
	/* Pointer to the actor that we're showing the components for. */
	TWeakObjectPtr<AActor> ActorContextPtr;

	/** Delegate to invoke on selection update. */
	FOnComponentSelectedSignature OnComponentSelected;

	/** Called when the 'clear selection' option is selected instead of another component. */
	FOnClearedSelectionSignature OnClearedSelection;

	/** Attribute to limit visible nodes to a particular component type when filtering the tree view. */
	TAttribute<TArray<TSubclassOf<UActorComponent>>> AllowedClassesFilter;

	/** Attribute to limit visible nodes to a particular component type when filtering the tree view. */
	TAttribute<TArray<TSubclassOf<UActorComponent>>> DisallowedClassesFilter;

	/* The internal tree view used to visualize the components. */
	TSharedPtr<SPaperZDComponentTreeView> TreeView;

	/* The list of root tree elements. */
	TArray<FPaperZDComponentTreeItemPtr> TreeViewDataSource;

	/* Header row of the table. */
	TSharedPtr<SHeaderRow> HeaderRowWidget;

	/* The text filter used by the search bar. */
	typedef TTextFilter<const FPaperZDComponentTreeItem&> TreeItemTextFilter;
	TSharedPtr<SPaperZDComponentSelector::TreeItemTextFilter> SearchBoxFilter;

	/* Widget containing the filtering text box */
	TSharedPtr<SSearchBox> FilterTextBoxWidget;

	/* Current sort mode of the header. */
	EColumnSortMode::Type CurrentSortMode;

	/* If the selector allows clearing, which will show an extra option. */
	bool bAllowClear;
};
