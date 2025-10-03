// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameDataRepository.h"
#include "GameDataRepositoryEntrySelector.h"
#include "Serialization/GameDataEntrySerializer.h"

class IGameDataEntrySerializer;
class UAssetGameDataRepository;

/**
 * A data structure representing a selected row in an editor or data repository.
 */
struct FSelectedRow
{
    /**
     * The index of a specific entry within a data repository or collection.
     * Used to identify and manipulate the associated data element.
     */
    int32 Index;

    /**
     * Represents the current name associated with a selected row or data entry.
     * Typically used for identification or referencing purposes.
     */
    FName CurrentName;

    /**
     * Constructs an FSelectedRow instance with the specified index and name.
     *
     * @param InIndex The index of the selected row within the data repository or collection.
     * @param InCurrentName The current name associated with the selected row or data entry.
     *                      Used for identification or referencing.
     */
    FSelectedRow(const int32 InIndex, const FName InCurrentName) : Index(InIndex), CurrentName(InCurrentName)
    {
    }
};

/**
 * Represents the editor implementation for managing a Game Data Repository asset.
 * Provides functionality for initializing the editor with the appropriate toolkit host,
 * managing tab spawners, and interacting with repository data entries.
 */
class GAMEDATAACCESSTOOLSEDITOR_API FGameDataRepositoryEditor final : public FAssetEditorToolkit
{
  public:
    /**
     * Initializes the game data repository editor with the specified toolkit mode, toolkit host, and asset.
     *
     * @param Mode The mode in which the editor toolkit is being initialized (e.g., standalone or within a level
     * editor).
     * @param InitToolkitHost The host for the toolkit, used for managing its integration within the editor.
     * @param Asset The asset representing the game data repository to be edited.
     */
    void Initialize(const EToolkitMode::Type Mode, const TSharedPtr<IToolkitHost> &InitToolkitHost,
                    UAssetGameDataRepository *Asset);
    void RegisterTabSpawners(const TSharedRef<FTabManager> &InTabManager) override;
    void UnregisterTabSpawners(const TSharedRef<FTabManager> &InTabManager) override;

    FName GetToolkitFName() const override;
    FText GetBaseToolkitName() const override;
    FString GetWorldCentricTabPrefix() const override;
    FLinearColor GetWorldCentricTabColorScale() const override;

  private:
    void FillToolbar(FToolBarBuilder &ToolbarBuilder);

    TSharedRef<SWidget> ImportMenuEntries();
    void ImportGameDataRepository(const UGameDataEntrySerializer *Serializer) const;

    bool CanAddEntry() const;
    bool CanMoveEntryUp() const;
    bool CanMoveEntryDown() const;
    bool CanDeleteEntry() const;

    void OnEntrySelected(const TSharedPtr<FEntryRowData> &Entry);
    TArray<TSharedPtr<FEntryRowData>> OnGetEntries() const;
    void OnAddEntry() const;
    void OnDeleteEntry();
    void OnMoveEntryUp();
    void OnMoveEntryDown();
    void RefreshList() const;
    void OnPropertyChanged(const FPropertyChangedEvent &PropertyChangedEvent);

    TObjectPtr<UAssetGameDataRepository> GameDataRepository;
    TSharedPtr<SGameDataRepositoryEntrySelector> EntrySelector;
    TSharedPtr<IStructureDetailsView> DetailsView;
    TOptional<FSelectedRow> CurrentRow;
    TArray<TObjectPtr<const UGameDataEntrySerializer>> Serializers;
};
