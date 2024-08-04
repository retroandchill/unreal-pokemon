// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "GridBasedCharacterUtilities.generated.h"

class IGridMovable;
/**
 *
 */
UCLASS()
class GRIDBASED2D_API UGridBasedCharacterUtilities : public UBlueprintFunctionLibrary {
    GENERATED_BODY()

  public:
    UFUNCTION(BlueprintCallable, Category = "Map|Grid", meta = (DefaultToSelf = "Character"))
    static TSet<FName> CollectComponentTagsForCurrentTile(ACharacter *Character);

    /**
     * Check if a tile is a valid floor tile
     * @param TargetSquare The world space location of the tile in question
     * @param HitComponent The actor component that was hit in the collision
     * @return Is there no valid floor at this height
     */
    UFUNCTION(BlueprintPure, Category = "Map|Grid",
              meta = (DefaultToSelf = "Character", AutoCreateRefTerm = "TargetSquare"))
    static bool InvalidFloor(ACharacter *Character, const FVector &TargetSquare,
                             const UPrimitiveComponent *HitComponent);

    /**
     * Check if a tile is a cliff face
     * @param TargetSquare The world space location of the tile in question
     * @return Is the target tile a cliff
     */
    UFUNCTION(BlueprintPure, Category = "Map|Grid",
              meta = (DefaultToSelf = "Character", AutoCreateRefTerm = "TargetSquare"))
    static bool IsStandingNextToCliff(ACharacter *Character, const FVector &TargetSquare);

    /**
     * Checks if the character can step up on the selected component at all
     * @param Component The component to check
     * @return Can the player step up?
     */
    static bool CanStepUpOnComponent(ACharacter *Character, const UPrimitiveComponent &Component);

    /**
     * Find the world space position just off of a tile's edge
     * @param TargetSquare The world space location of the tile in question
     * @return What is the world space location of one pixel off of the player's current tile
     */
    static FVector FindLocationJustOffTileEdge(ACharacter *Character, const FVector &TargetSquare);

    /**
     * Find the world space position just before a tile's edge
     * @param TargetSquare The world space location of the tile in question
     * @return What is the world space location of one pixel off of the player's current tile
     */
    static FVector FindLocationJustBeforeTileEdge(ACharacter *Character, const FVector &TargetSquare);

    /**
     * Compute the distance to the ground from the current position
     * @param Position The position to sweep to the ground from
     * @return The distance to the ground and the hit component
     */
    static TPair<double, UPrimitiveComponent *> PerformTraceToGround(ACharacter *Character, const FVector &Position);
};
