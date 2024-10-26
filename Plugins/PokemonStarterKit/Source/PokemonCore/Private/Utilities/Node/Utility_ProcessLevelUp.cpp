// "Unreal Pokémon" created by Retro & Chill.

#include "Utilities/Node/Utility_ProcessLevelUp.h"

DEFINE_INJECTABLE_DEPENDENCY(UUtility_ProcessLevelUp)

int32 FStatChange::Diff() const {
    return After - Before;
}

FLevelUpStatChanges &FLevelUpStatChanges::operator+=(const FLevelUpStatChanges &Other) {
    LevelChange.Before = FMath::Min(LevelChange.Before, Other.LevelChange.Before);
    LevelChange.After = FMath::Max(LevelChange.After, Other.LevelChange.After);
    for (auto &[ID, StatChange] : Other.StatChanges) {
        if (!StatChanges.Contains(ID)) {
            continue;
        }

        auto &[Before, After] = StatChanges.FindChecked(ID);
        Before = FMath::Min(Before, StatChange.Before);
        After = FMath::Max(After, StatChange.After);
    }

    return *this;
}

void UUtility_ProcessLevelUp::Execute(const TScriptInterface<IPokemon> &Pokemon, const FLevelUpStatChanges &StatChanges,
                                      bool bShowMessage, FSimpleDelegate &&OnEnd) {
    EndProcess.Add(std::move(OnEnd));
    Execute(Pokemon, StatChanges, bShowMessage);
}

void UUtility_ProcessLevelUp::ExecuteOnEnd() {
    EndProcess.Broadcast();
    Destruct();
}