// Copyright 2017 ~ 2023 Critical Failure Studio Ltd. All rights reserved.

#include "Editors/Slate/SPaperZDComponentTreeView.h"
#include "Kismet2/ComponentEditorUtils.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Views/SExpanderArrow.h"
#include "Widgets/Layout/SSeparator.h"
#include "Styling/SlateIconFinder.h"
#include "Styling/AppStyle.h"

#define LOCTEXT_NAMESPACE "SPaperZDComponentTreeView"

FPaperZDComponentTreeItem::FPaperZDComponentTreeItem()
	: bFiltered(false)
	, bHasRelevantChildren(false)
	, bInteractable(false)
	, bSeparatorItem(true)
	, Parent(nullptr)
{}

FPaperZDComponentTreeItem::FPaperZDComponentTreeItem(const FSubobjectDataHandle& InSubobjectDataSource)
	: bFiltered(false)
	, bHasRelevantChildren(false)
	, bInteractable(true)
	, bSeparatorItem(false)
	, Parent(nullptr)
{
	if (InSubobjectDataSource.GetSharedDataPtr().IsValid())
	{
		TSharedPtr<FSubobjectData> ObjectData = InSubobjectDataSource.GetSharedDataPtr();
		if (const UObject* Object = ObjectData->GetObject<UObject>())
		{
			Object->GetClass()->GetName(CachedClassString);
			CachedDisplayString = InSubobjectDataSource.GetSharedDataPtr()->GetVariableName().ToString();
		}

		//Store the actor component
		ComponentPtr = ObjectData->GetObject<UActorComponent>();
	}
}

FPaperZDComponentTreeItemPtr FPaperZDComponentTreeItem::MakeSeparatorItem()
{
	return MakeShareable(new FPaperZDComponentTreeItem());
}

void FPaperZDComponentTreeItem::AddChild(FPaperZDComponentTreeItemRef ChildItem)
{
	Children.Add(ChildItem);
	ChildItem->OnAddedAsChild(AsShared());
}

const TArray<FPaperZDComponentTreeItemPtr>& FPaperZDComponentTreeItem::GetChildren() const
{
	return Children;
}

void FPaperZDComponentTreeItem::SortChildren(bool bAscending, bool bSortDescendants /* = false */)
{
	Children.Sort([bAscending](const FPaperZDComponentTreeItemPtr& A, const FPaperZDComponentTreeItemPtr& B) 
	{
		return (A->GetDisplayString().Compare(B->GetDisplayString()) < 0) == bAscending;
	});

	if (bSortDescendants)
	{
		for (FPaperZDComponentTreeItemPtr Ptr : Children)
		{
			Ptr->SortChildren(bAscending, bSortDescendants);
		}
	}
}

const FString FPaperZDComponentTreeItem::GetDisplayString() const
{
	return CachedDisplayString;
}

const FString FPaperZDComponentTreeItem::GetClassDisplayString() const
{
	return CachedClassString;
}

bool FPaperZDComponentTreeItem::IsInteractable() const
{
	return ComponentPtr.IsValid() && bInteractable && !bFiltered && !bSeparatorItem;
}

bool FPaperZDComponentTreeItem::IsValidItem() const
{
	return ComponentPtr.IsValid() || bSeparatorItem;
}

bool FPaperZDComponentTreeItem::IsSeparator() const
{
	return bSeparatorItem;
}

bool FPaperZDComponentTreeItem::IsRelevant() const
{
	return !bFiltered || bHasRelevantChildren;
}

void FPaperZDComponentTreeItem::SetFilterClasses(FActorComponentSubclassArray AllowedClasses, FActorComponentSubclassArray DissallowedClasses)
{
	if (ComponentPtr.IsValid())
	{
		//Check if the component is part of the hierarchy of any of the filtered classes
		bInteractable = false;
		for (TSubclassOf<UActorComponent> DisallowedClass : DissallowedClasses)
		{
			if (ComponentPtr->IsA(DisallowedClass))
			{
				bInteractable = false;
				return;
			}
		}

		//If we haven't gotten to this point, then we haven't explicitly triggered a disallowed class
		for (TSubclassOf<UActorComponent> AllowedClass : AllowedClasses)
		{
			if (ComponentPtr->IsA(AllowedClass))
			{
				bInteractable = true;
				break;
			}
		}
	}
	else
	{
		bInteractable = false;
	}
}

void FPaperZDComponentTreeItem::SetFilteredOut(bool bFilteredOut)
{
	bFiltered = bFilteredOut;
	NotifyRelevancyToParent();
}

const UActorComponent* FPaperZDComponentTreeItem::GetComponent() const
{
	return ComponentPtr.Get();
}

void FPaperZDComponentTreeItem::NotifyRelevancyToParent()
{
	if (Parent.IsValid())
	{
		Parent.Pin()->OnChildNotifiedRelevancy(IsRelevant());
	}
}

void FPaperZDComponentTreeItem::OnChildNotifiedRelevancy(bool bRelevantChildren)
{
	bHasRelevantChildren |= bRelevantChildren;
	NotifyRelevancyToParent();	
}

void FPaperZDComponentTreeItem::OnAddedAsChild(FPaperZDComponentTreeItemRef InParent)
{
	Parent = InParent;
	NotifyRelevancyToParent();
}

//Tree row
/** Default metrics for the component tree view, copied from the outliner defaults to have a similar style. */
struct FDefaultViewMetrics
{
	static int32	RowHeight() { return 22; };
	static int32	IconSize() { return 16; };
	static FMargin	IconPadding() { return FMargin(0.f, 1.f, 6.f, 1.f); };
	static FLinearColor InactiveColor() { return FLinearColor(0.15f, 0.15f, 0.15f); }
};

void SPaperZDComponentTreeRow::Construct(const FArguments& Args, const TSharedRef<STableViewBase>& OwnerTableView)
{
	ItemPtr = Args._Item;

	auto SuperArgs = STableRow<FPaperZDComponentTreeItemPtr>::FArguments()
		.Style(&FAppStyle::Get().GetWidgetStyle<FTableRowStyle>("SceneOutliner.TableViewRow"));	//Mimic the SceneOutliner style
	STableRow<FPaperZDComponentTreeItemPtr>::Construct(SuperArgs, OwnerTableView);
	
	//Special case for separators
	if (ItemPtr.IsValid() && ItemPtr.Pin()->IsSeparator())
	{
		ChildSlot
		[
			SNew(SBox)
			.Padding(1.f)
			[
				SNew(SSeparator)
				.SeparatorImage(FAppStyle::Get().GetBrush("Menu.Separator"))
				.Thickness(1.f)
			]
		];
	}
	else
	{
		//Create the row body
		TSharedRef<SHorizontalBox> RowBody = SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			[
				SNew(SHorizontalBox)

				//Icon first
				+ SHorizontalBox::Slot()
				.AutoWidth()
				.VAlign(VAlign_Center)
				.Padding(FDefaultViewMetrics::IconPadding())
				[
					SNew(SBox)
					.WidthOverride(FDefaultViewMetrics::IconSize())
					.HeightOverride(FDefaultViewMetrics::IconSize())
					[
						SNew(SImage)
						.Image(this, &SPaperZDComponentTreeRow::GetIcon)
						//.ToolTipText(this, &SComponentTreeLabel::GetIconTooltip)
					]
				]

				//Text block
				+ SHorizontalBox::Slot()
				.FillWidth(1.0f)
				.VAlign(VAlign_Center)
				.Padding(0.0f, 2.0f)
				[
					SNew(STextBlock)
					.Text(this, &SPaperZDComponentTreeRow::GetDisplayText)
					.ColorAndOpacity(this, &SPaperZDComponentTreeRow::GetForegroundColor)
					//.IsSelected(FIsSelected::CreateSP(&InRow, &STableRow<FSceneOutlinerTreeItemPtr>::IsSelectedExclusively))
				]
			];
	
		//Fill visuals
		ChildSlot
		[
			SNew(SBox)
			.MinDesiredHeight(FDefaultViewMetrics::RowHeight())
			[
				SNew( SHorizontalBox )
					
				+SHorizontalBox::Slot()
				.AutoWidth()
				.Padding(6, 0, 0, 0)
				[
					SNew( SExpanderArrow, SharedThis(this) ).IndentAmount(12)
				]

				+SHorizontalBox::Slot()
				.FillWidth(1.0f)
				[
					RowBody
				]
			]
		];
	}
}

FReply SPaperZDComponentTreeRow::OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	//Override button press behavior to only work when the item is interactable. 
	if (ItemPtr.IsValid() && ItemPtr.Pin()->IsInteractable())
	{
		return STableRow<FPaperZDComponentTreeItemPtr>::OnMouseButtonDown(MyGeometry, MouseEvent);
	}

	return FReply::Handled();
}

FReply SPaperZDComponentTreeRow::OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	//Override button press behavior to only work when the item is interactable. 
	if (ItemPtr.IsValid() && ItemPtr.Pin()->IsInteractable())
	{
		return STableRow<FPaperZDComponentTreeItemPtr>::OnMouseButtonUp(MyGeometry, MouseEvent);
	}

	return FReply::Handled();
}

FText SPaperZDComponentTreeRow::GetDisplayText() const
{
	if (auto Item = ItemPtr.Pin())
	{
		return FText::FromString(Item->GetDisplayString());
	}

	//If there's nothing, simply return a fail string.
	return LOCTEXT("NoTreeRowName", "--None--");
}

FSlateColor SPaperZDComponentTreeRow::GetForegroundColor() const
{
	if (auto Item = ItemPtr.Pin())
	{
		return !Item->IsInteractable() ? FSlateColor(FDefaultViewMetrics::InactiveColor()) : FSlateColor::UseStyle();
	}

	return FSlateColor::UseForeground();
}

const FSlateBrush* SPaperZDComponentTreeRow::GetIcon() const
{
	if (auto Item = ItemPtr.Pin())
	{
		if (const UActorComponent* ActorComponent = Item->GetComponent())
		{
			return FSlateIconFinder::FindIconBrushForClass(ActorComponent->GetClass());
		}
		return FSlateIconFinder::FindIconBrushForClass(UActorComponent::StaticClass());
	}

	return nullptr;
}

#undef LOCTEXT_NAMESPACE