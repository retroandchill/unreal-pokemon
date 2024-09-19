// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Templates/NonNullSubclassOf.h"
#include "UObject/Object.h"

class UBattleItemEffect;
struct FItem;
class UGameplayAbility;

namespace Pokemon::Battle::Items {
    /**
     * Lookup a hold item effect by its ID
     * @param ID The ID that's associated with the hold item effect
     * @return The gameplay ability class for that item (if an effect exists)
     */
    POKEMONBATTLE_API TOptional<TNonNullSubclassOf<UGameplayAbility>> FindHoldItemEffect(FName ID);

    /**
     * Lookup a hold item effect by its ID
     * @param Item The hold item's data struct
     * @return The gameplay ability class for that item (if an effect exists)
     */
    POKEMONBATTLE_API TOptional<TNonNullSubclassOf<UGameplayAbility>> FindHoldItemEffect(const FItem *Item);

    /**
     * Lookup a hold item effect by its ID
     * @param ID The ID that's associated with the hold item effect
     * @return The gameplay ability class for that item (if an effect exists)
     */
    POKEMONBATTLE_API TSubclassOf<UBattleItemEffect> FindBattleItemEffect(FName ID);

    /**
     * Lookup a hold item effect by its ID
     * @param Item The hold item's data struct
     * @return The gameplay ability class for that item (if an effect exists)
     */
    POKEMONBATTLE_API TSubclassOf<UBattleItemEffect> FindBattleItemEffect(const FItem *Item);

} // namespace Pokemon::Battle::Items
