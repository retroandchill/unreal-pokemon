// "Unreal Pokémon" created by Retro & Chill.
#include "Pokemon/Stats/DefaultMainBattleStatEntry.h"
#include "Species/Nature.h"

void UDefaultMainBattleStatEntry::RefreshValue(int32 Level, int32 Base, const FNature &Nature) {
    auto Stat = GetID();
    auto NatureChange =
        Nature.StatChanges.FindByPredicate([&Stat](const FNatureStatChange &Change) { return Change.Stat == Stat; });
    int32 NatureModifer = NatureChange != nullptr ? 100 + NatureChange->Change : 100;

    SetStatValue(((2 * Base + GetIV() + GetEV() / 4) * Level / 100 + 5) * NatureModifer / 100);
}
