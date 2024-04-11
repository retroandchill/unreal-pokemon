// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "PaperTileReplacement.generated.h"
#include "UObject/Interface.h"

struct FPaperTileMetadata;
struct FPaperTileInfo;
// This class does not need to be modified.
UINTERFACE()
class UPaperTileReplacement : public UInterface {
    GENERATED_BODY()
};

/**
 * Interface to be used on actors that replace a Paper Tile in a TileMap.
 */
class TILEREPLACER_API IPaperTileReplacement {
    GENERATED_BODY()

    // Add interface functions to this class. This is the class that will be inherited to implement this interface.
  public:
    /**
     * Set the replacement material for this component
     * @param TileInfo The information regarding the particular tile in question
     * @param TileMetadata The metadata for the tile in question
     * @param bCollisionEnabled Is there collision on the given tile?
     */
    UFUNCTION(BlueprintImplementableEvent, Category = "Tile Replacement")
    void ConfigureReplacement(const FPaperTileInfo &TileInfo, const FPaperTileMetadata &TileMetadata,
                              bool bCollisionEnabled);
};
