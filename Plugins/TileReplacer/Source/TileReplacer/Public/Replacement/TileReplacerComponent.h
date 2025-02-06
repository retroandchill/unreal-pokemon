// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ReplacedTile.h"

#include "TileReplacerComponent.generated.h"

class ATileReplacementActor;
class UPaperTileMapComponent;
class UDataTable;

/**
 * Actor component used for replacing specific tiles in a tilemap with an animated actor instead.
 */
UCLASS(ClassGroup = ("Animated Tiles"), meta = (BlueprintSpawnableComponent))
class TILEREPLACER_API UTileReplacerComponent : public UActorComponent {
    GENERATED_BODY()

  public:
    /**
     * Sets default values for this component's properties.
     */
    UTileReplacerComponent();

    /**
     * Replace all specified tiles in the tilemap with the animated material.
     * <p></p>
     * <p><b>NOTE:</b> This is very inefficient, and should not be used during runtime. This should be called in the
     * Editor</p>
     * @param TilemapComponent The tilemap component to use.
     */
    void ReplaceTiles(UPaperTileMapComponent *TilemapComponent);

    /**
     * Revert all tile replacements and insert the old ones.
     * <p></p>
     * <p><b>NOTE:</b> This is very inefficient, and should not be used during runtime. This should be called in the
     * Editor</p>
     * @param TileMapComponent The tilemap component to use.
     */
    void RestoreCachedTiles(UPaperTileMapComponent *TileMapComponent);

  private:
    /**
     * The table used to look up tile replacements
     */
    UPROPERTY(EditAnywhere, Category = Tiles)
    TObjectPtr<UDataTable> TileReplacementTable;

    /**
     * A cache of replaced tiles used to restore the orginal tiles
     */
    UPROPERTY()
    TArray<FReplacedTile> ReplacedTiles;
};
