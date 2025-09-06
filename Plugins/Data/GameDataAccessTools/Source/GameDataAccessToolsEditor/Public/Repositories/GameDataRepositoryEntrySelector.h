// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

struct FEntryRowData
{
	int32 Index;
	FName Id;
	TSharedPtr<FStructOnScope> Entry;

	FEntryRowData(const int32 InIndex, const FName InName, TSharedPtr<FStructOnScope> InEntry)
		: Index(InIndex), Id(InName), Entry(MoveTemp(InEntry))
	{
	}

	bool operator==(const FEntryRowData& Other) const = default;
};

class SWrapBox;

DECLARE_DELEGATE_OneParam(FOnEntrySelected, const TSharedPtr<FEntryRowData>&);
DECLARE_DELEGATE_RetVal(TArray<TSharedPtr<FEntryRowData>>, FOnGetEntries);
DECLARE_DELEGATE(FOnAddEntry);
DECLARE_DELEGATE_OneParam(FOnDeleteEntry, const TSharedPtr<FEntryRowData>&);
DECLARE_DELEGATE_OneParam(FOnMoveEntryUp, const TSharedPtr<FEntryRowData>&);
DECLARE_DELEGATE_OneParam(FOnMoveEntryDown, const TSharedPtr<FEntryRowData>&);

/**
 *
 */
class GAMEDATAACCESSTOOLSEDITOR_API SGameDataRepositoryEntrySelector final : public SCompoundWidget
{
public:
    SLATE_BEGIN_ARGS(SGameDataRepositoryEntrySelector)
        {
        }

        SLATE_EVENT(FOnEntrySelected, OnEntrySelected)
        SLATE_EVENT(FOnGetEntries, OnGetEntries)
    SLATE_END_ARGS()

    /** Constructs this widget with InArgs */
    void Construct(const FArguments& InArgs);
    void RefreshList();
    void SelectAtIndex(int32 Index);

    const TArray<TSharedPtr<FEntryRowData>>& GetEntries() const;
    TArray<TSharedPtr<FEntryRowData>> GetSelectedEntries() const;
    bool IsFiltering() const;

private:
    TSharedRef<ITableRow> OnGenerateRow(TSharedPtr<FEntryRowData> Item, const TSharedRef<STableViewBase>& OwnerTable);
    void OnSearchTextChanged(const FText& InSearchText);
    void OnSelectionChanged(TSharedPtr<FEntryRowData> Item, ESelectInfo::Type SelectType) const;

    // UI Elements
    TSharedPtr<SSearchBox> SearchBox;
    TSharedPtr<SListView<TSharedPtr<FEntryRowData>>> EntriesList;

    // Data
    TArray<TSharedPtr<FEntryRowData>> AllEntries;
    TArray<TSharedPtr<FEntryRowData>> FilteredEntries;

    FOnEntrySelected OnEntrySelected;
    FOnGetEntries OnGetEntries;
};
