// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "PaperTileLayer.h"

#include "TileReplacement.generated.h"

class UPaperTileSet;

/**
 * Struct row for tile replacement data
 */
USTRUCT(BlueprintType)
struct TILEREPLACER_API FTileReplacement : public FTableRowBase {
    GENERATED_BODY()

    /**
     * The tile used as the source for the replacement
     */
    UPROPERTY(EditAnywhere, Category = "Tile Replacement")
    FPaperTileInfo SourceTile;

    /**
     * The actor to replace the tile with
     */
    UPROPERTY(EditAnywhere, Category = "Tile Replacement", meta = (MustImplement = "PaperTileReplacement"))
    TSubclassOf<AActor> TileReplacement;

    /**
     * Should the replaced tile have collision?
     */
    UPROPERTY(EditAnywhere, Category = "Tile Replacement")
    bool bCollisionEnabled = false;
};

TILEREPLACER_API uint32 GetTypeHash(const FPaperTileInfo &TileInfo);