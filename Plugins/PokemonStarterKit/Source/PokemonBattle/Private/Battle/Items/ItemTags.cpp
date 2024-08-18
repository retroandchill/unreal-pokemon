// "Unreal Pokémon" created by Retro & Chill.

#include "Battle/Items/ItemTags.h"
#include "NativeGameplayTags.h"

namespace Pokemon::Battle::Items {

    /**
     * Tag applied to a battler when they're actively using an item.
     */
    const UE_DEFINE_GAMEPLAY_TAG(UsingItem, "Battle.UsingItem");

} // namespace Pokemon::Battle::Items