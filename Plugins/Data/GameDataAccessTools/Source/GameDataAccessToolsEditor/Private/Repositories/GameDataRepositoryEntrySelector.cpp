// Fill out your copyright notice in the Description page of Project Settings.

#include "Repositories/GameDataRepositoryEntrySelector.h"
#include "SlateOptMacros.h"
#include "Widgets/Input/SSearchBox.h"

void SGameDataRepositoryEntrySelector::Construct(const FArguments &InArgs)
{
    OnEntrySelected = InArgs._OnEntrySelected;
    OnGetEntries = InArgs._OnGetEntries;

    ChildSlot[SNew(SVerticalBox)
              // Search bar
              + SVerticalBox::Slot().AutoHeight().Padding(
                    2)[SAssignNew(SearchBox, SSearchBox)
                           .OnTextChanged(this, &SGameDataRepositoryEntrySelector::OnSearchTextChanged)
                           .HintText(NSLOCTEXT("GameDataRepositoryEditor", "SearchBoxHint", "Search entries..."))]

              // Entries list
              + SVerticalBox::Slot().FillHeight(
                    1.f)[SAssignNew(EntriesList, SListView<TSharedPtr<FEntryRowData>>)
                             .ListItemsSource(&FilteredEntries)
                             .OnGenerateRow(this, &SGameDataRepositoryEntrySelector::OnGenerateRow)
                             .OnSelectionChanged(this, &SGameDataRepositoryEntrySelector::OnSelectionChanged)
                             .SelectionMode(ESelectionMode::Single)]];

    RefreshList();
}

void SGameDataRepositoryEntrySelector::RefreshList()
{
    if (OnGetEntries.IsBound())
    {
        AllEntries = OnGetEntries.Execute();
        FilteredEntries = AllEntries;
        EntriesList->RequestListRefresh();
    }
}

void SGameDataRepositoryEntrySelector::SelectAtIndex(const int32 Index)
{
    if (Index >= 0 && Index < AllEntries.Num())
    {
        EntriesList->SetSelection(AllEntries[Index]);
    }
}

const TArray<TSharedPtr<FEntryRowData>> &SGameDataRepositoryEntrySelector::GetEntries() const
{
    return AllEntries;
}

TArray<TSharedPtr<FEntryRowData>> SGameDataRepositoryEntrySelector::GetSelectedEntries() const
{
    return EntriesList->GetSelectedItems();
}

bool SGameDataRepositoryEntrySelector::IsFiltering() const
{
    return FilteredEntries.Num() < AllEntries.Num();
}

TSharedRef<ITableRow> SGameDataRepositoryEntrySelector::OnGenerateRow(TSharedPtr<FEntryRowData> Item,
                                                                      const TSharedRef<STableViewBase> &OwnerTable)
{
    return SNew(
        STableRow<TSharedPtr<FEntryRowData>>,
        OwnerTable)[SNew(SHorizontalBox)

                    // Index column
                    + SHorizontalBox::Slot().AutoWidth().Padding(5)[SNew(STextBlock).Text(FText::AsNumber(Item->Index))]

                    // Name column
                    + SHorizontalBox::Slot().FillWidth(1.0f).Padding(
                          5)[SNew(STextBlock).Text(FText::FromName(Item->Id))]];
}

void SGameDataRepositoryEntrySelector::OnSearchTextChanged(const FText &InSearchText)
{
    FilteredEntries.Empty();
    const FString SearchString = InSearchText.ToString();

    for (const auto &Entry : AllEntries)
    {
        if (SearchString.IsEmpty() || Entry->Id.ToString().Contains(SearchString))
        {
            FilteredEntries.Add(Entry);
        }
    }

    EntriesList->RequestListRefresh();
}

void SGameDataRepositoryEntrySelector::OnSelectionChanged(TSharedPtr<FEntryRowData> Item, ESelectInfo::Type) const
{
    if (OnEntrySelected.IsBound())
    {
        OnEntrySelected.Execute(Item);
    }
}