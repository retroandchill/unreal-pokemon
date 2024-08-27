#pragma once

#include "CoreMinimal.h"
#include "TileSet.h"
#include "Styling/SlateTypes.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/Views/STableRow.h"


class STileSetPalette;
class FGridMapEditorMode;
class FAssetThumbnailPool;

class FTileSetPaletteItemModel : public TSharedFromThis<FTileSetPaletteItemModel>
{
public:
	FTileSetPaletteItemModel(UGridMapTileSet* InTileSet, TSharedRef<STileSetPalette> InTileSetPalette, FGridMapEditorMode* InEditorMode);

private:
	UGridMapTileSet* TileSet;
	TWeakPtr<STileSetPalette> TileSetPalette;
	FGridMapEditorMode* EditorMode;
};

class STileSetItemTile : public STableRow<UGridMapTileSet*>
{
public:
	SLATE_BEGIN_ARGS(STileSetItemTile) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, TSharedRef<STableViewBase> InOwnerTableView, TSharedPtr<FAssetThumbnailPool> InThumbnailPool, UGridMapTileSet* TileSet);
};