// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "GridBased2DSettings.h"
#include "Subsystems/GameInstanceSubsystem.h"

#include "MapSubsystem.generated.h"

class IMapGrid;
class IGridMovable;
class UGridBasedMovementComponent;
class ULevelStreamingDynamic;
class AGameCharacter;
enum class EFacingDirection : uint8;
class IInteractable;

/**
 * Subsystem that handles the traversal between various maps as well as the music within a given map.
 */
UCLASS()
class GRIDBASED2D_API UMapSubsystem : public UGameInstanceSubsystem {
    GENERATED_BODY()

  public:
    /**
     * Initialize the default object
     * @param Initializer The Unreal initializer
     */
    explicit UMapSubsystem(const FObjectInitializer &Initializer);

    /**
     * Warp to the given map name and coordinates
     * @param Map The the map to warp to
     * @param WarpTag The X coordinate of the map to warp to
     */
    UFUNCTION(BlueprintCallable, DisplayName = "Warp to Map (Retain Direction)", Category = "Maps|Warping")
    void WarpToMap(const TSoftObjectPtr<UWorld> &Map, FName WarpTag);

    /**
     * Warp to the given map name and coordinates
     * @param Map The the map to warp to
     * @param WarpTag The X coordinate of the map to warp to
     * @param Direction The direction the character should be facing after the warp
     */
    UFUNCTION(BlueprintCallable, DisplayName = "Warp to Map (Change Direction)", Category = "Maps|Warping")
    void WarpToMapWithDirection(const TSoftObjectPtr<UWorld> &Map, FName WarpTag, EFacingDirection Direction);

    /**
     * Set the location of the player in the world if there is a valid warp destination
     * @param PlayerCharacter The character to set the location of
     */
    void SetPlayerLocation(const TScriptInterface<IGridMovable> &PlayerCharacter);

    /**
     * Update what map the player is considered to be a part of
     * @param Movable The movement object in question
     */
    void UpdateCharacterMapPosition(const TScriptInterface<IGridMovable> &Movable) const;

    /**
     * Get the destination that the player is set to warp to.
     * @return The destination that the player is set to warp to.
     */
    const TOptional<TPair<FName, EFacingDirection>> &GetWarpDestination() const;

  private:
    /**
     * If set, indicates that the player warping to another location
     */
    TOptional<TPair<FName, EFacingDirection>> WarpDestination;
};
