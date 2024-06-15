// "Unreal Pokémon" created by Retro & Chill.

#include "Battle/Effects/UserEffects/DamageEffects.h"
#include "Battle/Moves/BattleDamage.h"
#include "Battle/Moves/BattleMove.h"

void UEffectParentalBond::ModifyDamageForUser_Implementation(FDamageMultipliers &Multipliers,
                                                             const TScriptInterface<IBattler> &User,
                                                             const TScriptInterface<IBattler> &Target,
                                                             const TScriptInterface<IBattleMove> &Move,
                                                             int32 BaseDamage, FName MoveType) {
    Multipliers.PowerMultiplier /= 4;
}

void UEffectMeFirst::ModifyDamageForUser_Implementation(FDamageMultipliers &Multipliers,
                                                        const TScriptInterface<IBattler> &User,
                                                        const TScriptInterface<IBattler> &Target,
                                                        const TScriptInterface<IBattleMove> &Move, int32 BaseDamage,
                                                        FName MoveType) {
    Multipliers.PowerMultiplier *= 1.5f;
}

void UEffectHelpingHand::ModifyDamageForUser_Implementation(FDamageMultipliers &Multipliers,
                                                            const TScriptInterface<IBattler> &User,
                                                            const TScriptInterface<IBattler> &Target,
                                                            const TScriptInterface<IBattleMove> &Move, int32 BaseDamage,
                                                            FName MoveType) {
    if (Move->IsConfusionAttack()) {
        return;
    }

    Multipliers.PowerMultiplier *= 1.5f;
}

void UEffectCharge::ModifyDamageForUser_Implementation(FDamageMultipliers &Multipliers,
                                                       const TScriptInterface<IBattler> &User,
                                                       const TScriptInterface<IBattler> &Target,
                                                       const TScriptInterface<IBattleMove> &Move, int32 BaseDamage,
                                                       FName MoveType) {
    static const FName Electric = TEXT("ELECTRIC");
    if (MoveType == Electric) {
        Multipliers.PowerMultiplier *= 2;
    }
}