// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "PaperTileLayer.h"

#include "ReplacedTile.generated.h"

/**
 * Struct used to cache a replaced tile
 */
USTRUCT()
struct TILEREPLACER_API FReplacedTile
{
    GENERATED_BODY()

    /**
     * The original tileset and information about the tile.
     */
    UPROPERTY(EditAnywhere, Category = "Original Tile")
    FPaperTileInfo OriginalTileInfo;

    /**
     * The X-coordinate of the tile
     */
    UPROPERTY(EditAnywhere, Category = "Tile Position")
    int32 TileX;

    /**
     * The Y-coordinate of the tile
     */
    UPROPERTY(EditAnywhere, Category = "Tile Position")
    int32 TileY;

    /**
     * The layer of the tile
     */
    UPROPERTY(EditAnywhere, Category = "Tile Position")
    int32 TileLayer;

    /**
     * The actor that replaces the tile
     */
    UPROPERTY(EditAnywhere, Category = "Tile Position")
    TObjectPtr<AActor> Replacement;

    /**
     * Construct an empty replacement struct
     */
    FReplacedTile();

    /**
     * Construct with the given information
     * @param TileInfo The original tileset and information about the tile.
     * @param X The X-coordinate of the tile
     * @param Y The Y-coordinate of the tile
     * @param Layer The layer of the tile
     * @param Replacement The actor that replaces the tile
     */
    FReplacedTile(const FPaperTileInfo &TileInfo, int32 X, int32 Y, int32 Layer, AActor *Replacement = nullptr);
};
