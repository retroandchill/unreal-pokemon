// "Unreal Pokémon" created by Retro & Chill.

#include "Utilities/PokemonUtilities.h"

int32 FStatChange::Diff() const {
    return After - Before;
}

// Add default functionality here for any IPokemonUtilities functions that are not pure virtual.
FLevelUpStatChanges &FLevelUpStatChanges::operator+=(const FLevelUpStatChanges &Other) {
    LevelChange.Before = FMath::Min(LevelChange.Before, Other.LevelChange.Before);
    LevelChange.After = FMath::Max(LevelChange.After, Other.LevelChange.After);
    for (auto &[ID, StatChange] : Other.StatChanges) {
        auto &[Before, After] = StatChanges.FindChecked(ID);
        Before = FMath::Min(Before, StatChange.Before);
        After = FMath::Max(After, StatChange.After);
    }

    return *this;
}