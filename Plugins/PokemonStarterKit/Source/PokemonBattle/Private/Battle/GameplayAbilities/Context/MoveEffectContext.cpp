// "Unreal Pokémon" created by Retro & Chill.


#include "Battle/GameplayAbilities/Context/MoveEffectContext.h"

const FMoveDamageInfo & UMoveEffectContext::GetDamageInfo() const {
    return *DamageInfo;
}

void UMoveEffectContext::SetDamageInfo(const TSharedRef<FMoveDamageInfo> &NewDamageInfo) {
    DamageInfo = NewDamageInfo;
}