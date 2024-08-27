// Fill out your copyright notice in the Description page of Project Settings.


#include "TileSet.h"

UGridMapTileSet::UGridMapTileSet()
	: TileSize(100)
	, TileHeight(200)
{
}

TSoftObjectPtr<class UStaticMesh> FGridMapTileList::GetRandomTile() const
{
	if (Tiles.Num() == 0)
		return TSoftObjectPtr<class UStaticMesh>();

	int32 RandomElementIndex = FMath::RandRange(0, Tiles.Num() - 1);
	return Tiles[RandomElementIndex];
}

const FGridMapTileList* UGridMapTileSet::FindTilesForAdjacency(uint32 bitmask) const
{
	const FGridMapTileList* GridMapTileList = SearchForTilesWithCompatibleAdjacency(bitmask);
	// If we couldn't find a matching tile, we might be relying on 4 way
	// tiles, so let's mask off the upper bits and check again
	if (!GridMapTileList)
		GridMapTileList = SearchForTilesWithCompatibleAdjacency(bitmask & 0xF);
	return GridMapTileList;
}

const FGridMapTileList* UGridMapTileSet::SearchForTilesWithCompatibleAdjacency(uint32 bitmask) const
{
	static const TTuple<uint32, uint32> TopLeft((1 << 0) | (1 << 1), ~(1 << 4));
	static const TTuple<uint32, uint32> TopRight((1 << 0) | (1 << 2), ~(1 << 5));
	static const TTuple<uint32, uint32> BottomLeft((1 << 1) | (1 << 3), ~(1 << 6));
	static const TTuple<uint32, uint32> BottomRight((1 << 2) | (1 << 3), ~(1 << 7));

	for (int i = 0; i < Tiles.Num(); ++i)
	{
		uint32 filteredBitmask = bitmask;
		int32 tileBitmask = Tiles[i].TileAdjacency.Bitset;

		// filter out any values we don't care about
		if (!((tileBitmask & TopLeft.Key) == TopLeft.Key))
			filteredBitmask &= TopLeft.Value;
		if (!((tileBitmask & TopRight.Key) == TopRight.Key))
			filteredBitmask &= TopRight.Value;
		if (!((tileBitmask & BottomLeft.Key) == BottomLeft.Key))
			filteredBitmask &= BottomLeft.Value;
		if (!((tileBitmask & BottomRight.Key) == BottomRight.Key))
			filteredBitmask &= BottomRight.Value;

		if (tileBitmask == filteredBitmask)
			return &Tiles[i];
	}

	return nullptr;
}