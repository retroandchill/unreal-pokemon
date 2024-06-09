// "Unreal Pokémon" created by Retro & Chill.


#include "Battle/Abilities/Effects/AuraAbilities.h"
#include "Battle/Battle.h"
#include "Battle/Moves/BattleDamage.h"
#include "Battle/Moves/BattleMove.h"

static bool FindAuraBreak(const TScriptInterface<IBattle>& Battle) {
    static const FName AuraBreak = TEXT("AURABREAK");
    return Battle->FindGlobalAbility(AuraBreak);
}

static void ApplyAuraEffect(FDamageMultipliers& Multipliers, FName MoveType, FName TargetType, const TScriptInterface<IBattle>& Battle) {
    if (MoveType != TargetType) {
        return;
    }
    if (FindAuraBreak(Battle)) {
        Multipliers.FinalDamageMultiplier *= 3 / 4.f;
    } else {
        Multipliers.FinalDamageMultiplier *= 4 / 3.0f;
    }
}

void UDarkAura::TriggerDamageCalcFromGlobal_Implementation(FDamageMultipliers &Multipliers,
                                                           const TScriptInterface<IBattler> &User, const TScriptInterface<IBattler> &Target,
                                                           const TScriptInterface<IBattleMove> &Move, int32 BaseDamage, FName MoveType) {
    static const FName Dark = TEXT("DARK");
    ApplyAuraEffect(Multipliers, MoveType, Dark, IBattleMove::Execute_GetOwningBattle(Move.GetObject()));
}

void UFairyAura::TriggerDamageCalcFromGlobal_Implementation(FDamageMultipliers &Multipliers,
    const TScriptInterface<IBattler> &User, const TScriptInterface<IBattler> &Target,
    const TScriptInterface<IBattleMove> &Move, int32 BaseDamage, FName MoveType) {
    static const FName Dark = TEXT("FAIRY");
    ApplyAuraEffect(Multipliers, MoveType, Dark, IBattleMove::Execute_GetOwningBattle(Move.GetObject()));
}