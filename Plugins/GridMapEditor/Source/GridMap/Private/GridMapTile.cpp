// "Unreal Pokémon" created by Retro & Chill.


#include "GridMapTile.h"
#include "TileSet.h"


// Add default functionality here for any IGridMapTile functions that are not pure virtual.
bool FGridTileHandle::IsValidTile() const {
    return IsValid(TileSet) && TileSet->Tiles.IsValidIndex(TileIndex);
}

FGameplayTagContainer FGridTileHandle::GetTileTags() const {
    if (!IsValidTile()) {
        return FGameplayTagContainer();
    }

    auto BaseTags = TileSet->TileTags;
    BaseTags.AppendTags(TileSet->Tiles[TileIndex].TileTags);
    return BaseTags;
}