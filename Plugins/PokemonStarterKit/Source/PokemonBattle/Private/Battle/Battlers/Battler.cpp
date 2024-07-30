// "Unreal Pokémon" created by Retro & Chill.

#include "Battle/Battlers/Battler.h"

// Add default functionality here for any IBattler functions that are not pure virtual.
FStatusEffectInfo::FStatusEffectInfo(FName StatusEffectID, FActiveGameplayEffectHandle EffectHandle)
    : StatusEffectID(StatusEffectID), EffectHandle(EffectHandle) {
}

FExpGainInfo::FExpGainInfo(const TScriptInterface<IBattler> &Battler, int32 Amount, bool bBoosted)
    : GainingBattler(Battler), Amount(Amount), bBoosted(bBoosted) {
}