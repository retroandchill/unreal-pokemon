// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "GridBasedMapBase.h"
#include "LevelBoundsGridBasedMap.generated.h"

/**
 * Grid-based map actor that uses computations on a level bounds actor (or any actor really) to perform its computations.
 */
UCLASS(Blueprintable, ClassGroup = (Map))
class GRIDBASED2D_API ALevelBoundsGridBasedMap : public AGridBasedMapBase {
    GENERATED_BODY()

protected:
    FIntRect GetBounds() const override;

private:
    /**
     * Level bounds actor, must be valid or Play-in-Editor will be denied. In a packaged game it will crash.
     */
    UPROPERTY(EditInstanceOnly, Category = "Bounds")
    TObjectPtr<AActor> LevelBounds;

};