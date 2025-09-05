// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameDataRepository.h"
#include "GameDataRepositoryDataAccessor.h"
#include "GameDataRepositoryEntrySelector.h"
#include "Serialization/GameDataEntrySerializer.h"

class IGameDataEntrySerializer;
class UGameDataRepository;

struct FSelectedRow
{
	int32 Index;
	FName CurrentName;

	FSelectedRow(const int32 InIndex, const FName InCurrentName)
		: Index(InIndex), CurrentName(InCurrentName)
	{
	}
};

/**
 *
 */
class GAMEDATAACCESSTOOLSEDITOR_API FGameDataRepositoryEditor final : public FAssetEditorToolkit
{
public:
	void Initialize(const EToolkitMode::Type Mode, const TSharedPtr<IToolkitHost>& InitToolkitHost,
	                UGameDataRepository* Asset);
	void RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager) override;
	void UnregisterTabSpawners(const TSharedRef<FTabManager>& InTabManager) override;

	FName GetToolkitFName() const override;
	FText GetBaseToolkitName() const override;
	FString GetWorldCentricTabPrefix() const override;
	FLinearColor GetWorldCentricTabColorScale() const override;

private:
	void FillToolbar(FToolBarBuilder& ToolbarBuilder);

	TSharedRef<SWidget> ImportMenuEntries();
	void ImportGameDataRepository(FGameDataEntrySerializerPtr Serializer) const;

	bool CanAddEntry() const;
	bool CanMoveEntryUp() const;
	bool CanMoveEntryDown() const;
	bool CanDeleteEntry() const;

	void OnEntrySelected(const TSharedPtr<FEntryRowData>& Entry);
	TArray<TSharedPtr<FEntryRowData>> OnGetEntries() const;
	void OnAddEntry() const;
	void OnDeleteEntry();
	void OnMoveEntryUp();
	void OnMoveEntryDown();
	void RefreshList() const;
	void OnPropertyChanged(const FPropertyChangedEvent& PropertyChangedEvent);

	TSharedPtr<SGameDataRepositoryEntrySelector> EntrySelector;
	TSharedPtr<IStructureDetailsView> DetailsView;
	TUniquePtr<FGameDataRepositoryDataAccessor> PropertyAccessor;
	TOptional<FSelectedRow> CurrentRow;
	TArray<FGameDataEntrySerializerPtr> Serializers;
};
