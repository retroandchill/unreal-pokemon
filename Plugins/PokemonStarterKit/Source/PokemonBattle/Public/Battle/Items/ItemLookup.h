// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"

struct FItem;
class UGameplayAbility;

namespace Pokemon::Battle::Items {
/**
 * Lookup a hold item effect by its ID
 * @param ID The ID that's associated with the hold item effect
 * @return The gameplay ability class for that item (if an effect exists)
 */
POKEMONBATTLE_API TSubclassOf<UGameplayAbility> FindHoldItemEffect(FName ID);

/**
 * Lookup a hold item effect by its ID
 * @param Item The hold item's data struct
 * @return The gameplay ability class for that item (if an effect exists)
 */
POKEMONBATTLE_API TSubclassOf<UGameplayAbility> FindHoldItemEffect(const FItem* Item);

}
