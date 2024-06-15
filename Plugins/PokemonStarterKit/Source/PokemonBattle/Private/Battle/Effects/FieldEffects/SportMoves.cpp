// "Unreal Pokémon" created by Retro & Chill.

#include "Battle/Effects/FieldEffects/SportMoves.h"
#include "Battle/Moves/BattleDamage.h"

static void ApplySportEffect(FDamageMultipliers &Multipliers, FName MoveType, FName TargetType) {
    if (MoveType == TargetType) {
        Multipliers.PowerMultiplier /= 3.0f;
    }
}

void UEffectMudSport::ModifyDamage_Implementation(FDamageMultipliers &Multipliers,
                                                  const TScriptInterface<IBattler> &User,
                                                  const TScriptInterface<IBattler> &Target,
                                                  const TScriptInterface<IBattleMove> &Move, int32 BaseDamage,
                                                  FName MoveType) {
    static const FName Electric = TEXT("ELECTRIC");
    ApplySportEffect(Multipliers, MoveType, Electric);
}

void UEffectWaterSport::ModifyDamage_Implementation(FDamageMultipliers &Multipliers,
                                                    const TScriptInterface<IBattler> &User,
                                                    const TScriptInterface<IBattler> &Target,
                                                    const TScriptInterface<IBattleMove> &Move, int32 BaseDamage,
                                                    FName MoveType) {
    static const FName Fire = TEXT("FIRE");
    ApplySportEffect(Multipliers, MoveType, Fire);
}