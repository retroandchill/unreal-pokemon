// "Unreal Pokémon" created by Retro & Chill.
#include "Pokemon/Stats/DefaultMainStatEntry.h"

FDefaultMainStatEntry::FDefaultMainStatEntry(FName Stat, int32 IV, int32 EV) : FDefaultStatEntry(Stat, IV, EV) {
}

void FDefaultMainStatEntry::RefreshValue(int32 Level, int32 Base, const FNature&) {
	SetStatValue((2 * Base + GetIV() + GetEV() / 4) * Level / 100 + Level + 10);
}

TUniquePtr<IStatEntry> FDefaultMainStatEntry::Clone() const {
	return MakeUnique<FDefaultMainStatEntry>(*this);
}
