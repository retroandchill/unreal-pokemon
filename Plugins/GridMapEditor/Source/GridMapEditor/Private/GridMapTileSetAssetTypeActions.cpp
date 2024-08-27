#include "GridMapTileSetAssetTypeActions.h"
#include "ContentBrowserModule.h"
#include "GridMapTileSetFactory.h"
#include "TileSet.h"
#include "ToolMenuSection.h"

#define LOCTEXT_NAMESPACE "GridMapEditor"

FGridMapTileSetAssetTypeActions::FGridMapTileSetAssetTypeActions(EAssetTypeCategories::Type InAssetCategory)
	: AssetCategory(InAssetCategory)
{
}

FText FGridMapTileSetAssetTypeActions::GetName() const
{
	return LOCTEXT("FGridMapTileSetAssetTypeActionsName", "Grid Tiles");
}

FColor FGridMapTileSetAssetTypeActions::GetTypeColor() const
{
	return FColorList::Orange;
}

UClass* FGridMapTileSetAssetTypeActions::GetSupportedClass() const
{
	return UGridMapTileSet::StaticClass();
}

uint32 FGridMapTileSetAssetTypeActions::GetCategories()
{
	return AssetCategory;
}
