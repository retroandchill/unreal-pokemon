// "Unreal Pokémon" created by Retro & Chill.
#include "Pokemon/Stats/StatBlock.h"

DEFINE_INJECTABLE_DEPENDENCY(IStatBlock)

int32 FStatChange::Diff() const
{
    return After - Before;
}

FLevelUpStatChanges &FLevelUpStatChanges::operator+=(const FLevelUpStatChanges &Other)
{
    LevelChange.Before = FMath::Min(LevelChange.Before, Other.LevelChange.Before);
    LevelChange.After = FMath::Max(LevelChange.After, Other.LevelChange.After);
    for (auto &[ID, StatChange] : Other.StatChanges)
    {
        if (!StatChanges.Contains(ID))
        {
            continue;
        }

        auto &[Before, After] = StatChanges.FindChecked(ID);
        Before = FMath::Min(Before, StatChange.Before);
        After = FMath::Max(After, StatChange.After);
    }

    return *this;
}