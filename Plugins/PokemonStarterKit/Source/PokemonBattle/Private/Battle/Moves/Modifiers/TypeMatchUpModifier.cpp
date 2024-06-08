// "Unreal Pokémon" created by Retro & Chill.


#include "Battle/Moves/Modifiers/TypeMatchUpModifier.h"
#include "DataManager.h"
#include "Battle/Type.h"
#include "Battle/Battlers/Battler.h"
#include "Battle/Moves/BaseBattleMove.h"
#include "Battle/Moves/BattleMove.h"

void Battle::Moves::FTypeMatchUpModifier::ApplyModifier(FModifiedDamage& Damage,
                                                        const UBaseBattleMove* Move, const TScriptInterface<IBattler> &User,
                                                        const TScriptInterface<IBattler> &Target, int32 TargetCount) const {
    using enum EDamageEffectiveness;
    float Effectiveness = 1.0f;
    auto &TypeTable = FDataManager::GetInstance().GetDataTable<FType>();
    auto MoveType = Move->DetermineType();
    TArray<FType*> Types;
    Algo::Transform(Target->GetTypes(), Types, [&TypeTable](FName Type) { return TypeTable.GetData(Type); });
    for (auto Type : Types) {
        if (Type->Immunities.Contains(MoveType)) {
            Damage = {.Modifier = 0.f, .Effectiveness = NoEffect };
            return;
        }

        if (Type->Weaknesses.Contains(MoveType)) {
            Effectiveness *= 2.f;
        } else if (Type->Resistances.Contains(MoveType)) {
            Effectiveness /= 2.f;
        }
    }
    
    if (Effectiveness > 1.f) {
        Damage.Effectiveness = SuperEffective;
    } else if (Effectiveness < 1.f) {
        Damage.Effectiveness = NotVeryEffective;
    }
    Damage.Modifier *= Effectiveness;
}