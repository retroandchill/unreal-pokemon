#pragma once

#include "CoreMinimal.h"
#include "Layout/Visibility.h"
#include "Styling/SlateTypes.h"
#include "Types/SlateEnums.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/Views/STileView.h"

class FGridMapEditorMode;
class FAssetThumbnailPool;
class UGridMapTileSet;

typedef STileView<UGridMapTileSet*> SGridMapTileSetTileView;

class STileSetPalette : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(STileSetPalette) {}
		SLATE_ARGUMENT(FGridMapEditorMode*, GridMapEditorMode)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);
	~STileSetPalette();

private:
	TSharedRef<SWidget> GetAddTileSetPicker();
	TSharedRef<SWidget> BuildPaletteView();

	TSharedRef<ITableRow> GenerateTile(UGridMapTileSet* Item, const TSharedRef<STableViewBase>& OwnerTable);
	void OnSelectionChanged(UGridMapTileSet* Item, ESelectInfo::Type SelectInfo);

	void UpdatePalette(bool bRebuildItems);
	EActiveTimerReturnType UpdatePaletteItems(double InCurrentTime, float InDeltaTime);
	void RefreshPalette();
	EActiveTimerReturnType RefreshPaletteItems(double InCurrentTime, float InDeltaTime);



	void AddTileSet(const struct FAssetData& AssetData);
	void OnSearchTextChanged(const FText& InFilterText);

private:
	TSharedPtr<class SComboButton> AddTileSetCombo;
	TSharedPtr<class SSearchBox> SearchBoxPtr;
	TSharedPtr<class FAssetThumbnailPool> ThumbnailPool;
	TSharedPtr<SGridMapTileSetTileView> TileViewWidget;

	TArray<UGridMapTileSet* > FilteredItems;



	bool bItemsNeedRebuild;
	bool bIsRebuildTimerRegistered;
	bool bIsRefreshTimerRegistered;

	FGridMapEditorMode* EditorMode;
};