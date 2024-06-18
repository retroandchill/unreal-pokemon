// "Unreal Pokémon" created by Retro & Chill.


#include "CriticalTestMove.h"

ECriticalOverride UCriticalTestMove::GetCriticalOverride_Implementation(const TScriptInterface<IBattler> &User,
    const TScriptInterface<IBattler> &Target) const {
    return CriticalOverride;
}