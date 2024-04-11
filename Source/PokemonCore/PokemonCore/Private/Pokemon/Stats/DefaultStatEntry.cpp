// "Unreal Pokémon" created by Retro & Chill.
#include "Pokemon/Stats/DefaultStatEntry.h"

#include "DataManager.h"
#include "DataTypes/OptionalUtilities.h"
#include "Pokemon/Stats/StatUtils.h"
#include "Species/Stat.h"

TScriptInterface<IStatEntry> UDefaultStatEntry::Initialize(
	FName Stat, const TOptional<int32>& IVs, int32 EVs) {
	StatID = Stat;
	IV = OrElseGet(IVs, [] { return StatUtils::RandomizeIV(); });
	EV = EVs;
	return this;
}

int32 UDefaultStatEntry::GetStatValue() const {
	return Value;
}

const FStat& UDefaultStatEntry::GetStat() const {
	auto Stat = FDataManager::GetInstance().GetDataTable<FStat>().GetData(StatID);
	check(Stat != nullptr)

	return *Stat;
}

FName UDefaultStatEntry::GetStatID() const {
	return StatID;
}

int32 UDefaultStatEntry::GetIV() const {
	return IV;
}

int32 UDefaultStatEntry::GetEV() const {
	return EV;
}

void UDefaultStatEntry::RefreshValue(int32 Level, int32 Base, const FNature& Nature) {
	// No implementation but abstracts are not allowed so we have to put this here
}

void UDefaultStatEntry::SetStatValue(int32 NewValue) {
	Value = NewValue;
}
