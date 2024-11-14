// Copyright 2017 ~ 2023 Critical Failure Studio Ltd. All rights reserved.

#include "Editors/Slate/SPaperZDComponentSelector.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Views/SHeaderRow.h"
#include "Widgets/Input/SSearchBox.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "SubobjectDataSubsystem.h" 	
#include "SubobjectDataHandle.h"
#include "Styling/AppStyle.h"

#define LOCTEXT_NAMESPACE "SPaperZDComponentSelector"

namespace ComponentSelectorConstants
{
	static const FVector2D ContentBrowserWindowSize(300.0f, 300.0f);
	static const FVector2D SceneOutlinerWindowSize(350.0f, 300.0f);

	static const FName ColName("ColName");
};

void SPaperZDComponentSelector::Construct(const FArguments& InArgs, AActor* InActorContext)
{
	ActorContextPtr = InActorContext;
	OnComponentSelected = InArgs._OnComponentSelected;
	OnClearedSelection = InArgs._OnClearedSelection;
	AllowedClassesFilter = InArgs._AllowedClassesFilter;
	DisallowedClassesFilter = InArgs._DisallowedClassesFilter;
	CurrentSortMode = EColumnSortMode::Ascending;
	bAllowClear = InArgs._AllowClear;

	//Create the view overlay
	TSharedRef<SVerticalBox> VerticalBox = SNew(SVerticalBox);	
	
	//Bind and create the text filter
	SearchBoxFilter = MakeShared<SPaperZDComponentSelector::TreeItemTextFilter>(TreeItemTextFilter::FItemToStringArray::CreateSP(this, &SPaperZDComponentSelector::PopulateSearchStrings));
	SearchBoxFilter->OnChanged().AddSP(this, &SPaperZDComponentSelector::RebuildTreeView);

	//Create the search box itself.
	FilterTextBoxWidget = SNew(SSearchBox)
		.Visibility(EVisibility::Visible)
		.HintText(LOCTEXT("FilterSearch", "Search..."))
		.ToolTipText(LOCTEXT("FilterSearchHint", "Type here to search"))
		.OnTextChanged(this, &SPaperZDComponentSelector::OnFilterTextChanged);

	//Add the search to the view
	VerticalBox->AddSlot()
	.AutoHeight()
	.Padding( 8.0f, 8.0f, 8.0f, 4.0f )
	[
		FilterTextBoxWidget.ToSharedRef()
	];

	//Header row
	HeaderRowWidget = SNew(SHeaderRow)
		.Visibility(EVisibility::Visible)
		//Add the default name column
		+ SHeaderRow::Column(ComponentSelectorConstants::ColName).DefaultLabel(LOCTEXT("HeaderLabel", "Item Label"))
		.SortMode(this, &SPaperZDComponentSelector::GetHeaderSortMode)
		.OnSort(this, &SPaperZDComponentSelector::OnColumnSortModeChanged);

	//Create the tree view
	TreeView = SNew(SPaperZDComponentTreeView)
		.SelectionMode(ESelectionMode::Single)
		.TreeItemsSource(&TreeViewDataSource)
		.OnGenerateRow(this, &SPaperZDComponentSelector::OnGenerateRowForComponentTree)
		.OnGetChildren(this, &SPaperZDComponentSelector::OnGetChildrenForComponentTree)
		.OnSelectionChanged(this, &SPaperZDComponentSelector::OnTreeSelectionChanged)
		.HeaderRow(HeaderRowWidget);

	//Add the tree view with some paddings
	VerticalBox->AddSlot()
	.AutoHeight()
	[
		SNew(SBox)
		.WidthOverride(ComponentSelectorConstants::SceneOutlinerWindowSize.X)
		.HeightOverride(ComponentSelectorConstants::SceneOutlinerWindowSize.Y)
		[
			SNew(SBorder)
			.BorderImage(FAppStyle::GetBrush("Menu.Background"))
			[
				TreeView.ToSharedRef()
			]
		]		
	];

	//Generate the tree view
	RebuildTreeView();

	//Add any extra options here
	FMenuBuilder MenuBuilder(true, NULL);
	MenuBuilder.BeginSection(NAME_None, LOCTEXT("OptionsHeader", "Options"));
	{
		//Clear
		MenuBuilder.AddMenuEntry(
			LOCTEXT("ClearComponent", "Clear"),
			LOCTEXT("ClearComponent_ToolTip", "Clears the component set on this field"),
			FSlateIcon(),
			FUIAction(FExecuteAction::CreateSP(this, &SPaperZDComponentSelector::OnClear), FCanExecuteAction::CreateSP(this, &SPaperZDComponentSelector::CanClear))
		);
	}

	//This is to make it a little similar to the SceneOutliner itself (which people are used to)
	MenuBuilder.BeginSection(NAME_None, LOCTEXT("BrowseHeader", "Select a component"));
	{
		MenuBuilder.AddWidget(VerticalBox, FText::GetEmpty(), true);
	}
	MenuBuilder.EndSection();

	//Commit to the child slot
	ChildSlot
	[
		MenuBuilder.MakeWidget()
	];

}

void SPaperZDComponentSelector::RebuildTreeView()
{
	TreeViewDataSource.Empty();
	if (ActorContextPtr.IsValid())
	{
		// Gather the data from the subobject subsystem
		USubobjectDataSubsystem* DataSubsystem = USubobjectDataSubsystem::Get();
		check(DataSubsystem);

		TArray<FSubobjectDataHandle> SubobjectData;
		DataSubsystem->GatherSubobjectData(ActorContextPtr.Get(), SubobjectData);

		//Translate subobject data to the expected item structure
		TArray<FSubobjectDataHandle> SceneComponentHandles;
		TMap<const USceneComponent*, FPaperZDComponentTreeItemRef> SceneComponentMap;
		TArray<FPaperZDComponentTreeItemRef> NonSceneCompItems;
		TArray<TSubclassOf<UActorComponent>> AllowedClasses = AllowedClassesFilter.Get(TArray<TSubclassOf<UActorComponent>>());
		TArray<TSubclassOf<UActorComponent>> DisallowedClasses = DisallowedClassesFilter.Get(TArray<TSubclassOf<UActorComponent>>());
		for (const FSubobjectDataHandle& Data : SubobjectData)
		{
			FPaperZDComponentTreeItemRef Ref = MakeShared<FPaperZDComponentTreeItem>(Data);			
			if (Ref->IsValidItem())
			{
				//Apply the filters if needed
				Ref->SetFilterClasses(AllowedClasses, DisallowedClasses);
				Ref->SetFilteredOut(!SearchBoxFilter->PassesFilter(Ref.Get()));

				//Check if this is a scene component
				if (const USceneComponent* Comp = Ref->GetComponent<USceneComponent>())
				{
					//Some handles get duplicated and we need to make sure we aren't adding them twice.
					if (!SceneComponentMap.Contains(Comp))
					{
						SceneComponentMap.Add(Comp, Ref);
						SceneComponentHandles.Add(Data);
					}
				}
				else
				{
					NonSceneCompItems.Add(Ref);
				}

			}
		}

		//We have the scene components separated and can re-build hierarchy now
		for (const FSubobjectDataHandle& Data : SceneComponentHandles)
		{
			const USceneComponent* Comp = Data.GetSharedDataPtr()->GetObject<USceneComponent>();
			FPaperZDComponentTreeItemRef Ref = SceneComponentMap[Comp];

			//Check if we have a parent, and if we have, that it is a scene component (and not the actor)
			const FSubobjectData* ParentData = Data.GetSharedDataPtr()->GetParentData();
			if (ParentData != nullptr )
			{
				//Check for the parent node 
				const USceneComponent* ParentComponent = ParentData->GetObject<USceneComponent>();
				if (ParentComponent)
				{
					FPaperZDComponentTreeItemRef ParentRef = SceneComponentMap[ParentComponent];
					ParentRef->AddChild(Ref);
					TreeView->SetItemExpansion(ParentRef, true);
				}
				else
				{
					//Root node
					TreeViewDataSource.Add(Ref);
				}
			}
		}

		//We need to filter out the root nodes, as these will end up being created by the table view
		//every child will be filtered out when sending the children of each item instead.
		//
		//We wait until the hierarchy is fully formed to have a grasp on the relevancy of every item and its descendants.
		auto FilterRelevantOnly = [](const FPaperZDComponentTreeItemPtr Ptr)
		{
			//Do not add if filtered out
			return Ptr->IsRelevant();
		};
		TreeViewDataSource = TreeViewDataSource.FilterByPredicate(FilterRelevantOnly);
		NonSceneCompItems = NonSceneCompItems.FilterByPredicate(FilterRelevantOnly);
		
		//Add a separator item and then the non-scene component items
		if (NonSceneCompItems.Num() > 0)
		{
			TreeViewDataSource.Add(FPaperZDComponentTreeItem::MakeSeparatorItem());
			TreeViewDataSource.Append(MoveTemp(NonSceneCompItems));
		}
	}

	SortTreeDataSource();
	TreeView->RequestTreeRefresh();
}

TSharedRef<ITableRow> SPaperZDComponentSelector::OnGenerateRowForComponentTree(FPaperZDComponentTreeItemPtr Item, const TSharedRef<STableViewBase>& OwnerTable)
{
	return SNew(SPaperZDComponentTreeRow, OwnerTable)
				.Item(Item);
}

void SPaperZDComponentSelector::OnGetChildrenForComponentTree(FPaperZDComponentTreeItemPtr InParent, TArray<FPaperZDComponentTreeItemPtr>& OutChildren)
{
	OutChildren = InParent->GetChildren().FilterByPredicate([](const FPaperZDComponentTreeItemPtr Item)
	{
		return Item->IsRelevant();
	});
}

void SPaperZDComponentSelector::PopulateSearchStrings(const FPaperZDComponentTreeItem& Item, TArray<FString>& OutSearchStrings) const
{
	OutSearchStrings.Add(Item.GetDisplayString());
	OutSearchStrings.Add(Item.GetClassDisplayString());
}

void SPaperZDComponentSelector::OnFilterTextChanged(const FText& InFilterText)
{
	SearchBoxFilter->SetRawFilterText(InFilterText);
}

EColumnSortMode::Type SPaperZDComponentSelector::GetHeaderSortMode() const
{
	return CurrentSortMode;
}

void SPaperZDComponentSelector::OnColumnSortModeChanged(const EColumnSortPriority::Type SortPriority, const FName& ColumnId, const EColumnSortMode::Type InSortMode)
{
	CurrentSortMode = InSortMode;
	SortTreeDataSource();
	TreeView->RequestTreeRefresh();
}

void SPaperZDComponentSelector::OnTreeSelectionChanged(FPaperZDComponentTreeItemPtr TreeItem, ESelectInfo::Type SelectInfo)
{
	if (TreeItem->IsValidItem())
	{
		OnComponentSelected.ExecuteIfBound(TreeItem->GetComponent(), *TreeItem->GetDisplayString());
	}
}

void SPaperZDComponentSelector::SortTreeDataSource()
{
	for (FPaperZDComponentTreeItemPtr Ptr : TreeViewDataSource)
	{
		const bool bAscending = CurrentSortMode == EColumnSortMode::Ascending;
		Ptr->SortChildren(bAscending, true);
	}
}

void SPaperZDComponentSelector::OnClear()
{
	OnClearedSelection.ExecuteIfBound();
}

bool SPaperZDComponentSelector::CanClear() const
{
	return bAllowClear;
}

#undef LOCTEXT_NAMESPACE