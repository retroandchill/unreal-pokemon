// "Unreal Pokémon" created by Retro & Chill.
#include "Battle/Type.h"
#include "DataManager.h"

float FType::Effectiveness(FName OtherType) const {
    if (Weaknesses.Contains(OtherType)) {
        return Pokemon::TypeEffectiveness::SuperEffectiveMultiplier;
    }

    if (Resistances.Contains(OtherType)) {
        return Pokemon::TypeEffectiveness::NotVeryEffectiveMultiplier;
    }

    if (Immunities.Contains(OtherType)) {
        return Pokemon::TypeEffectiveness::IneffectiveMultiplier;
    }

    return Pokemon::TypeEffectiveness::NormalEffectiveMultiplier;
}

TArray<FName> UTypeHelper::GetTypeNames() {
    return FDataManager::GetInstance().GetDataTable<FType>().GetTableRowNames();
}

float UTypeHelper::GetTypeEffectiveness(FName AttackType, FName DefendType) {
    static auto& TypeTable = FDataManager::GetInstance().GetDataTable<FType>();
    auto Type = TypeTable.GetData(DefendType);
    check(Type != nullptr)
    return Type->Effectiveness(AttackType);
}

EDamageEffectiveness UTypeHelper::GetEffectivenessForMultiplier(float Multiplier) {
    return Pokemon::TypeEffectiveness::GetEffectivenessFromMultiplier(Multiplier);
}
