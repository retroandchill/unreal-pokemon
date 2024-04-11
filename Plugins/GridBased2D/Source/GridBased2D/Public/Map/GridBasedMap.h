// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GridBasedMapBase.h"
#include "PaperTileMapComponent.h"
#include "GridBasedMap.generated.h"

class IGridMovable;
class AGameCharacter;
class UTileReplacerComponent;
class IWithinMap;

UCLASS(Blueprintable, ClassGroup = (Map))
class GRIDBASED2D_API AGridBasedMap : public AGridBasedMapBase {
    GENERATED_BODY()

  public:
    /**
     * Sets default values for this actor's properties
     */
    AGridBasedMap();

  protected:
    void PostInitProperties() override;
    void PostReinitProperties() override;
    void PostEditChangeProperty(FPropertyChangedEvent &PropertyChangedEvent) override;
    void PostLoad() override;
    void PostEditMove(bool bFinished) override;

  public:
#if WITH_EDITORONLY_DATA
    /**
     * Refresh the tiles, replacing any tiles that need to be replaced
     */
    UFUNCTION(BlueprintCallable, CallInEditor, Category = "Tiles")
    void RefreshTileData();

    /**
     * Clear all tile replacements, restoring the original tiles to their rightful place.
     */
    UFUNCTION(BlueprintCallable, CallInEditor, Category = "Tiles")
    void ClearTileReplacements();
#endif

    FIntRect GetBounds() const override;

  private:
    /**
     * Set up the location of the map based on the configured properties
     * @param bFinishedMoving Has moving the actor finished
     */
    void SetUpMapLocation(bool bFinishedMoving = true);

    /**
     * The tilemap to use for this asset
     */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Map, meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UPaperTileMapComponent> TileMapComponent;

#if WITH_EDITORONLY_DATA
    /**
     * The component used to replace tiles with animated/autotiles
     */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Map, meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UTileReplacerComponent> TileReplacer;
#endif

    /**
     * The layer of the tilemap that is at the same level as the player
     */
    UPROPERTY(EditAnywhere, Category = "Z-Sorting", meta = (UIMin = 0, ClampMin = 0))
    int32 PlayerLevelLayer = 1;

    /**
     * The data table asset used for terrain tag mappings, for various movement types and other traits
     */
    UPROPERTY(EditAnywhere, Category = "Tilesets")
    TObjectPtr<UDataTable> TerrainTagDataTable;
};
