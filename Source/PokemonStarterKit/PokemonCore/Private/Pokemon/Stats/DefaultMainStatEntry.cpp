// "Unreal Pokémon" created by Retro & Chill.
#include "Pokemon/Stats/DefaultMainStatEntry.h"

void UDefaultMainStatEntry::RefreshValue(int32 Level, int32 Base, const FNature &)
{
    SetStatValue((2 * Base + GetIV() + GetEV() / 4) * Level / 100 + Level + 10);
}
