#include "Widgets/TileSetPaletteItem.h"
#include "AssetThumbnail.h"
#include "EditorStyleSet.h"
#include "GridMapEditorMode.h"
#include "TileSet.h"
#include "Widgets/STileSetPalette.h"

FTileSetPaletteItemModel::FTileSetPaletteItemModel(UGridMapTileSet *InTileSet,
                                                   TSharedRef<STileSetPalette> InTileSetPalette,
                                                   FGridMapEditorMode *InEditorMode)
    : TileSet(InTileSet), TileSetPalette(InTileSetPalette), EditorMode(InEditorMode) {
}

void STileSetItemTile::Construct(const FArguments &InArgs, const FGridMapEditorMode &EditorMode,
                                 TSharedRef<STableViewBase> InOwnerTableView,
                                 TSharedPtr<FAssetThumbnailPool> InThumbnailPool, UGridMapTileSet *TileSet) {
    FAssetData AssetData(TileSet);
    int32 MaxThumbnailSize = 64;
    TSharedPtr<FAssetThumbnail> Thumbnail =
        MakeShareable(new FAssetThumbnail(AssetData, MaxThumbnailSize, MaxThumbnailSize, InThumbnailPool));

    auto GetHighlightColor = [&EditorMode, TileSet] {
        return EditorMode.UISettings.GetCurrentTileSet().Get() == TileSet ? FLinearColor::White
                                                                          : FLinearColor(0.5f, 0.5f, 0.5f, 1.f);
    };

    FAssetThumbnailConfig ThumbnailConfig;
    STableRow<UGridMapTileSet *>::Construct(
        STableRow<UGridMapTileSet *>::FArguments()
            .Style(FAppStyle::Get(), "ContentBrowser.AssetListView.ColumnListTableRow")
            .Padding(1.f)
            .Content()[SNew(SBorder)
                           .Padding(4.f)
                           .BorderImage(FAppStyle::GetBrush("ContentBrowser.ThumbnailShadow"))
                           .ForegroundColor(FLinearColor::White)
                           .ColorAndOpacity_Lambda(GetHighlightColor)[Thumbnail->MakeThumbnailWidget(ThumbnailConfig)]],
        InOwnerTableView);
}