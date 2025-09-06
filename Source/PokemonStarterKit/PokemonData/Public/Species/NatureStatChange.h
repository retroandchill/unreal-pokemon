// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"

#include "NatureStatChange.generated.h"

/**
 * Stores the stat change of a Nature
 */
USTRUCT(BlueprintType)
struct POKEMONDATA_API FNatureStatChange
{
    GENERATED_BODY()

    /**
     * The stat to alter
     */
    UPROPERTY(BlueprintReadOnly, EditAnywhere)
    FName Stat;

    /**
     * The percentage to change the stat by. Positive means add that percentage to the stat. Negative means substract
     * that percentage from the stat.
     */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (UIMin = -100, ClampMin = -100, UIMax = 100, ClampMax = 100))
    int32 Change;
};
