// Copyright 2017 ~ 2023 Critical Failure Studio Ltd. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Views/STreeView.h"
#include "Templates/SharedPointer.h"
#include "UObject/WeakObjectPtrTemplates.h"
#include "SubobjectDataSubsystem.h"
#include "Misc/Optional.h"

class AActor;
class UActorComponent;
class FPaperZDComponentTreeItem;
struct FSlateBrush;
struct FSubobjectDataHandle;

//typedefs for easier use
typedef TSharedPtr<FPaperZDComponentTreeItem> FPaperZDComponentTreeItemPtr;
typedef TSharedRef<FPaperZDComponentTreeItem> FPaperZDComponentTreeItemRef;
typedef STreeView<FPaperZDComponentTreeItemPtr> SPaperZDComponentTreeView;
typedef TArray<TSubclassOf<UActorComponent>> FActorComponentSubclassArray;

/**
 * An item in the component tree view
 */
class FPaperZDComponentTreeItem : public TSharedFromThis<FPaperZDComponentTreeItem>
{
	/* The component we're servicing. */
	TWeakObjectPtr<const UActorComponent> ComponentPtr;

	/* If this item has been filtered out. */
	bool bFiltered;

	/* If this item has relevant items that should be shown, even when this item itself has been filtered. */
	bool bHasRelevantChildren;

	/* If this item is interactable or not. */
	bool bInteractable;

	/* If this is actually the separator item instead. */
	bool bSeparatorItem;

	/* The parent of this node, if it exists. */
	TWeakPtr<FPaperZDComponentTreeItem> Parent;

	/* A list of the children of this node. */
	TArray<FPaperZDComponentTreeItemPtr> Children;

	/* Cached variable name. */
	FString CachedDisplayString;

	/* Cached variable name. */
	FString CachedClassString;

private:
	/* We want this to still exist so we can build separator items. */
	FPaperZDComponentTreeItem();

public:
	/* ctor. */
	FPaperZDComponentTreeItem(const FSubobjectDataHandle& SubobjectDataSource);

	/* Create a version of this item that represents a separator on the table view. */
	static FPaperZDComponentTreeItemPtr MakeSeparatorItem();

	/* Add a tree item as a child of this one. */
	void AddChild(FPaperZDComponentTreeItemRef ChildItem);

	/* Get the childrens of this component. */
	const TArray<FPaperZDComponentTreeItemPtr>& GetChildren() const;

	/* Sort the children either in an ascending or descending manner. */
	void SortChildren(bool bAscending, bool bSortDescendants = false);

	/* Obtains the display string of the element being referenced by this node. */
	const FString GetDisplayString() const;

	/* Obtains the display string of the element being referenced by this node. */
	const FString GetClassDisplayString() const;

	/* True if the item is interactable on the component selection widget. */
	bool IsInteractable() const;
	
	/* If this points to a valid item that can be processed. */
	bool IsValidItem() const;

	/* If this points to a valid item that can be processed. */
	bool IsSeparator() const;

	/* If this item is relevant and should be displayed in the tree view (not filtered, or has a descendant that isn't filtered). */
	bool IsRelevant() const;

	/* Sets the filter  class of this element, which defines whether the item is interactable or not. */
	void SetFilterClasses(FActorComponentSubclassArray AllowedClasses, FActorComponentSubclassArray DissalowedClasses);

	/* Sets this item to be filtered out or not (search bar filters). */
	void SetFilteredOut(bool bFilteredOut);

	/* Obtain the actor component that defines this item. */
	const UActorComponent* GetComponent() const;

	/* Obtains the component that defines this item, casted. */
	template <typename T>
	const T* GetComponent() const
	{
		return Cast<T>(ComponentPtr.Get());
	}

private:
	/* Calls the parent if it exists and notifies it of the relevancy of this node. Recursively notifies up. */
	void NotifyRelevancyToParent();

	/* Called by a child to notify this node that there's at least one item down the hierarchy that should be displayed. */
	void OnChildNotifiedRelevancy(bool bRelevantChildren);

	/* Called when a parent adds this item as a child. */
	void OnAddedAsChild(FPaperZDComponentTreeItemRef Parent);
};

/**
 * A row from the component tree view
 */
class SPaperZDComponentTreeRow final : public STableRow<FPaperZDComponentTreeItemPtr>
{
public:
	SLATE_BEGIN_ARGS(SPaperZDComponentTreeRow)
	{}
	/* List item for this row. */
	SLATE_ARGUMENT(FPaperZDComponentTreeItemPtr, Item)

	SLATE_END_ARGS()

	/* Constructor */
	void Construct(const FArguments& Args, const TSharedRef<STableViewBase>& OwnerTableView);

	/* Override mouse button to change intractability. */
	virtual FReply OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual FReply OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;

private:
	/* The list item we're representing. */
	TWeakPtr<const FPaperZDComponentTreeItem> ItemPtr;

	/* Display text from the component being referenced. */
	FText GetDisplayText() const;

	/* Obtain the foreground color on the item. */
	virtual FSlateColor GetForegroundColor() const override;

	/* Icon representing the row. */
	const FSlateBrush* GetIcon() const;
};
