// "Unreal Pokémon" created by Retro & Chill.


#include "Battle/Traits/CriticalHits/SimpleCriticalHitModificationTrait.h"

int32 USimpleCriticalHitModificationTrait::Apply_Implementation(TScriptInterface<IBattler> &User,
    const TScriptInterface<IBattler> &Target) const {
    return Change;
}