// "Unreal Pokémon" created by Retro & Chill.
#include "Pokemon/Stats/DefaultMainBattleStatEntry.h"

#include "Species/Nature.h"


FDefaultMainBattleStatEntry::FDefaultMainBattleStatEntry(FName Stat, uint32 PersonalityValue, const TOptional<int32>& IV, int32 EV) : FDefaultStatEntry(
	Stat, PersonalityValue, IV, EV) {
}

void FDefaultMainBattleStatEntry::RefreshValue(int32 Level, int32 Base, const FNature& Nature) {
	auto Stat = GetStatID();
	auto NatureChange = Nature.StatChanges.FindByPredicate([&Stat](const FNatureStatChange& Change) {
		return Change.Stat == Stat;
	});
	int32 NatureModifer = NatureChange != nullptr ? 100 + NatureChange->Change : 100;

	SetStatValue(((2 * Base + GetIV() + GetEV() / 4) * Level / 100 + 5) * NatureModifer / 100);
}

TUniquePtr<IStatEntry> FDefaultMainBattleStatEntry::Clone() const {
	return MakeUnique<FDefaultMainBattleStatEntry>(*this);
}
