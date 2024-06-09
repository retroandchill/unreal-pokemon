// "Unreal Pokémon" created by Retro & Chill.


#include "Battle/Effects/TargetEffects/EffectMinimize.h"
#include "Battle/Moves/BattleMove.h"

void UEffectMinimize::ModifyDamageForTarget_Implementation(FDamageMultipliers &Multipliers,
                                                           const TScriptInterface<IBattler> &User, const TScriptInterface<IBattler> &Target,
                                                           const TScriptInterface<IBattleMove> &Move, int32 BaseDamage, FName MoveType) {
    static const FName TramplesMinimize = TEXT("TramplesMinimize");
    if (Move->HasTag(TramplesMinimize)) {
        Multipliers.FinalDamageMultiplier *= 2;
    }
}