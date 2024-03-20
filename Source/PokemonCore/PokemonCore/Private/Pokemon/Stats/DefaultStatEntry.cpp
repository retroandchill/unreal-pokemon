// "Unreal Pokémon" created by Retro & Chill.
#include "Pokemon/Stats/DefaultStatEntry.h"

#include "Asserts.h"
#include "DataManager.h"
#include "DataTypes/OptionalUtilities.h"
#include "Pokemon/Stats/StatUtils.h"
#include "Species/Stat.h"

IMPLEMENT_DERIVED_METATYPE(FDefaultStatEntry)

FDefaultStatEntry::FDefaultStatEntry(FName Stat, uint32 PersonalityValue, const TOptional<int32>& IV, int32 EV) :
	StatID(Stat), PersonalityValue(PersonalityValue), IV(OrElseGet(IV, [] { return StatUtils::RandomizeIV(); })), EV(EV) {
}

int32 FDefaultStatEntry::GetStatValue() const {
	return Value;
}

const FStat& FDefaultStatEntry::GetStat() const {
	auto Stat = FDataManager::GetInstance().GetDataTable<FStat>().GetData(StatID);
	ASSERT(Stat != nullptr)

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

bool FDefaultStatEntry::operator==(const FDefaultStatEntry& Other) const {
	return StatID == Other.StatID && IV == Other.IV && EV == Other.EV;
}

bool FDefaultStatEntry::Equals(const IStatEntry& Other) const {
	if (GetClassName() != Other.GetClassName()) {
		return false;
	}

	return *this == static_cast<const FDefaultStatEntry &>(Other);
}

void FDefaultStatEntry::SetStatValue(int32 NewValue) {
	Value = NewValue;
}
