// "Unreal Pokémon" created by Retro & Chill.
#include "Pokemon/Stats/DefaultStatEntry.h"

#include "DataManager.h"
#include "Species/Stat.h"


FDefaultStatEntry::FDefaultStatEntry(FName Stat, int32 IV, int32 EV) : StatID(Stat), IV(IV), EV(EV) {
}

int32 FDefaultStatEntry::GetStatValue() const {
	return Value;
}

const FStat& FDefaultStatEntry::GetStat() const {
	auto Stat = FDataManager::GetInstance().GetDataTable<FStat>().GetData(StatID);
	check(Stat != nullptr);

	return *Stat;
}

FName FDefaultStatEntry::GetStatID() const {
	return StatID;
}

int32 FDefaultStatEntry::GetIV() const {
	return IV;
}

int32 FDefaultStatEntry::GetEV() const {
	return EV;
}

void FDefaultStatEntry::SetStatValue(int32 NewValue) {
	Value = NewValue;
}
