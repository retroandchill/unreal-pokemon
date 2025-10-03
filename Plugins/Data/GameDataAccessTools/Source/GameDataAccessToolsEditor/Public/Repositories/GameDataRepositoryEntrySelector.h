// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

class SWrapBox;

/**
 * Represents the data structure for a single row entry.
 */
struct FEntryRowData
{
    /**
     * Represents the index of a specific entry within a data repository or collection.
     * Used to uniquely identify and locate the associated data element.
     */
    int32 Index;

    /**
     * Represents the unique identifier for an entry.
     * Used to reference and distinguish the entry within the repository.
     */
    FName Id;

    /**
     * A shared pointer to a structured data object that exists within a custom scope.
     * Used to pass, store, or interact with structured data dynamically at runtime.
     */
    TSharedPtr<FStructOnScope> Entry;

    /**
     * Constructs a new instance of FEntryRowData with the specified index, name, and entry data.
     *
     * @param InIndex The index of the entry within the data repository or collection.
     * @param InName The unique identifier or name of the entry.
     * @param InEntry A shared pointer to a structured data object representing the entry's associated data.
     *                This is moved into the internal structure.
     * @return A new instance of FEntryRowData initialized with the provided parameters.
     */
    FEntryRowData(const int32 InIndex, const FName InName, TSharedPtr<FStructOnScope> InEntry)
        : Index(InIndex), Id(InName), Entry(MoveTemp(InEntry))
    {
    }

    /**
     * Compares two FEntryRowData instances for equality.
     *
     * @param Other The other instance of FEntryRowData to compare with.
     * @return true if both instances are equal; otherwise, false.
     */
    bool operator==(const FEntryRowData &Other) const = default;
};

DECLARE_DELEGATE_OneParam(FOnEntrySelected, const TSharedPtr<FEntryRowData> &);
DECLARE_DELEGATE_RetVal(TArray<TSharedPtr<FEntryRowData>>, FOnGetEntries);
DECLARE_DELEGATE(FOnAddEntry);
DECLARE_DELEGATE_OneParam(FOnDeleteEntry, const TSharedPtr<FEntryRowData> &);
DECLARE_DELEGATE_OneParam(FOnMoveEntryUp, const TSharedPtr<FEntryRowData> &);
DECLARE_DELEGATE_OneParam(FOnMoveEntryDown, const TSharedPtr<FEntryRowData> &);

/**
 * Represents a selector mechanism for entries within the game data repository.
 * Provides functionality to identify and interact with specific repository entries
 * based on defined criteria or conditions.
 */
class GAMEDATAACCESSTOOLSEDITOR_API SGameDataRepositoryEntrySelector final : public SCompoundWidget
{
  public:
    SLATE_BEGIN_ARGS(SGameDataRepositoryEntrySelector)
        {
        }

        SLATE_EVENT(FOnEntrySelected, OnEntrySelected)
        SLATE_EVENT(FOnGetEntries, OnGetEntries)
        SLATE_END_ARGS();

        /**
         * Constructs the SGameDataRepositoryEntrySelector UI widget.
         *
         * @param InArgs Arguments provided to initialize the widget, including callbacks for entry selection
         *               and obtaining the list of entries.
         */
        void Construct(const FArguments &InArgs);

        /**
         * Updates the displayed list of selectable entries by refreshing the data.
         *
         * This method retrieves the current set of entries from the delegate bound
         * to OnGetEntries, updates the internal data storage for both all entries
         * and filtered entries, and requests a UI list refresh to reflect any changes.
         * If the OnGetEntries delegate is not bound, no operation is performed.
         */
        void RefreshList();

        /**
         * Selects an entry at the specified index within the list of all available entries.
         *
         * If the provided index is valid (greater than or equal to 0 and less than the total number
         * of entries), the entry at the specified index will be set as the selected entry
         * in the list view.
         *
         * @param Index The index of the entry to select in the entries list.
         *              Must be a non-negative integer and within the range of valid entries.
         */
        void SelectAtIndex(int32 Index);

        /**
         * Retrieves the list of all entries currently stored in the selector.
         *
         * @return A constant reference to an array of shared pointers to FEntryRowData objects.
         *         This array contains all the entries available in the selector.
         */
        const TArray<TSharedPtr<FEntryRowData>> &GetEntries() const;

        /**
         * Retrieves the currently selected entries in the repository entry selector.
         *
         * @return An array of shared pointers to FEntryRowData objects representing the selected entries.
         */
        TArray<TSharedPtr<FEntryRowData>> GetSelectedEntries() const;

        /**
         * Determines if the repository entry selector is currently filtering its entries.
         *
         * Filtering occurs when the number of displayed (filtered) entries is less
         * than the total number of available entries.
         *
         * @return True if the selector is filtering the entries; otherwise, false.
         */
        bool IsFiltering() const;

      private:
        TSharedRef<ITableRow> OnGenerateRow(TSharedPtr<FEntryRowData> Item,
                                            const TSharedRef<STableViewBase> &OwnerTable);
        void OnSearchTextChanged(const FText &InSearchText);
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
