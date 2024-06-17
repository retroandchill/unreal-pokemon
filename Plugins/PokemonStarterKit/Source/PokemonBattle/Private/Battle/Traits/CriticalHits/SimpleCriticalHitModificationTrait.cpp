// "Unreal Pokémon" created by Retro & Chill.


#include "Battle/Traits/CriticalHits/SimpleCriticalHitModificationTrait.h"

void USimpleCriticalHitModificationTrait::Apply_Implementation(int32 &Value, ECriticalOverride& Override,
                                                               TScriptInterface<IBattler> &User, const TScriptInterface<IBattler> &Target) const {
    Value += Change;
}