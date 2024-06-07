// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"

/**
 * Meta info about a pocket in the bag.
 */
USTRUCT(BlueprintType)
struct FPocketInfo {
    GENERATED_BODY()

    /**
     * The name of the pocket as displayed to the player.
     */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bag")
    FText DisplayName;

    /**
     * The maximum number of items that can be held within a given pocket.
     */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bag", meta = (UIMin = 1, ClampMin = 1))
    TOptional<int32> MaxPocketSize;

    /**
     * Should the pocket be automatically sorted when items are added
     */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bag")
    bool bAutoSort;
};