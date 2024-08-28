#include "Widgets/TileSetPaletteItem.h"
#include "AssetThumbnail.h"
#include "EditorStyleSet.h"
#include "TileSet.h"
#include "Widgets/STileSetPalette.h"

FTileSetPaletteItemModel::FTileSetPaletteItemModel(UGridMapTileSet *InTileSet,
                                                   TSharedRef<STileSetPalette> InTileSetPalette,
                                                   FGridMapEditorMode *InEditorMode)
    : TileSet(InTileSet), TileSetPalette(InTileSetPalette), EditorMode(InEditorMode) {
}

void STileSetItemTile::Construct(const FArguments &InArgs, TSharedRef<STableViewBase> InOwnerTableView,
                                 TSharedPtr<FAssetThumbnailPool> InThumbnailPool, UGridMapTileSet *TileSet) {
    FAssetData AssetData(TileSet);
    int32 MaxThumbnailSize = 64;
    TSharedPtr<FAssetThumbnail> Thumbnail =
        MakeShareable(new FAssetThumbnail(AssetData, MaxThumbnailSize, MaxThumbnailSize, InThumbnailPool));

    FAssetThumbnailConfig ThumbnailConfig;
    STableRow<UGridMapTileSet *>::Construct(
        STableRow<UGridMapTileSet *>::FArguments()
            .Style(FEditorStyle::Get(), "ContentBrowser.AssetListView.TableRow")
            .Padding(1.f)
            .Content()[SNew(SBorder)
                           .Padding(4.f)
                           .BorderImage(FEditorStyle::GetBrush("ContentBrowser.ThumbnailShadow"))
                           .ForegroundColor(FLinearColor::White)
                               //.ColorAndOpacity(this, &SFoliagePaletteItemTile::GetTileColorAndOpacity)
                               [Thumbnail->MakeThumbnailWidget(ThumbnailConfig)]],
        InOwnerTableView);
}