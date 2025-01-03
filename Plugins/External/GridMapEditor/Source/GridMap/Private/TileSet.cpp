// Fill out your copyright notice in the Description page of Project Settings.

#include "TileSet.h"
#include "RetroLib/Optionals/OrElse.h"

UGridMapTileSet::UGridMapTileSet() : TileSize(100), TileHeight(200) {
}

TSoftObjectPtr<class UStaticMesh> FGridMapTileList::GetRandomTile() const {
    if (Tiles.Num() == 0)
        return TSoftObjectPtr<class UStaticMesh>();

    int32 RandomElementIndex = FMath::RandRange(0, Tiles.Num() - 1);
    return Tiles[RandomElementIndex];
}

TOptional<const FGridMapTileList &> UGridMapTileSet::FindTilesForAdjacency(uint32 Bitmask) const {
    // If we're using a tileset with a single tile, then assume it can be placed anywhere
    if (Tiles.Num() == 1) {
        return Tiles[0];
    }

    // If we couldn't find a matching tile, we might be relying on 4 way
    // tiles, so let's mask off the upper bits and check again
    return SearchForTilesWithCompatibleAdjacency(Bitmask) |
           Retro::Optionals::OrElse<&UGridMapTileSet::SearchForTilesWithCompatibleAdjacency>(Retro::TThis(this), Bitmask & 0xF);
}

TOptional<const FGridMapTileList &> UGridMapTileSet::SearchForTilesWithCompatibleAdjacency(uint32 Bitmask) const {
    static const TTuple<uint32, uint32> TopLeft((1 << 0) | (1 << 1), ~(1 << 4));
    static const TTuple<uint32, uint32> TopRight((1 << 0) | (1 << 2), ~(1 << 5));
    static const TTuple<uint32, uint32> BottomLeft((1 << 1) | (1 << 3), ~(1 << 6));
    static const TTuple<uint32, uint32> BottomRight((1 << 2) | (1 << 3), ~(1 << 7));

    for (int i = 0; i < Tiles.Num(); ++i) {
        uint32 FilteredBitmask = Bitmask;
        int32 TileBitmask = Tiles[i].TileAdjacency.Bitset;

        // filter out any values we don't care about
        if (!((TileBitmask & TopLeft.Key) == TopLeft.Key))
            FilteredBitmask &= TopLeft.Value;
        if (!((TileBitmask & TopRight.Key) == TopRight.Key))
            FilteredBitmask &= TopRight.Value;
        if (!((TileBitmask & BottomLeft.Key) == BottomLeft.Key))
            FilteredBitmask &= BottomLeft.Value;
        if (!((TileBitmask & BottomRight.Key) == BottomRight.Key))
            FilteredBitmask &= BottomRight.Value;

        if (TileBitmask == FilteredBitmask)
            return &Tiles[i];
    }

    return nullptr;
}