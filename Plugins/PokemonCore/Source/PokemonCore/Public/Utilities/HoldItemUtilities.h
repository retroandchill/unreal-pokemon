// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Bag/Item.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "HoldItemUtilities.generated.h"

class IPokemon;

UENUM(BlueprintInternalUseOnly)
enum class EHoldItemResult : uint8 {
    /**
     * This is executed when the Pokémon is holding an item.
     */
    HasItem,

    /**
     * This is executed when the Pokémon is not holding an item.
     */
    NoHeldItem
};

/**
 * Utility class used to get the hold item of a given Pokémon.
 */
UCLASS()
class POKEMONCORE_API UHoldItemUtilities : public UBlueprintFunctionLibrary {
    GENERATED_BODY()

  public:
    /**
     * Get a Pokémon's held item.
     * @param Pokemon The Pokémon to get the item for
     * @param HeldItem The structure of the found held item (this is undefined when there is no held item)
     * @param Branches The enum to represent the branches that are formed
     */
    UFUNCTION(BlueprintCallable, Category = Items, meta = (ExpandEnumAsExecs = "Branches", DefaultToSelf = "Pokemon"))
    static void GetHeldItem(const TScriptInterface<IPokemon> &Pokemon, FItem &HeldItem, EHoldItemResult &Branches);
};
