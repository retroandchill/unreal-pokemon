// "Unreal Pokémon" created by Retro & Chill.
#include "Pokemon/Stats/DefaultStatBlock.h"

#include "DataManager.h"
#include "Managers/PokemonSubsystem.h"
#include "Pokemon/Pokemon.h"
#include "Pokemon/PokemonDTO.h"
#include "Pokemon/Stats/DefaultMainBattleStatEntry.h"
#include "Pokemon/Stats/DefaultMainStatEntry.h"
#include "Pokemon/Stats/StatUtils.h"
#include "Species/Nature.h"
#include "Species/SpeciesData.h"
#include "Species/Stat.h"

using namespace StatUtils;

/**
 * Helper function to find a nature by the given ID
 * @param Nature The nature to look up
 * @return The found nature
 */
const FNature &FindNature(FName Nature) {
    const auto &DataSubsystem = FDataManager::GetInstance();
    auto &NatureTable = DataSubsystem.GetDataTable<FNature>();

    auto Ret = NatureTable.GetData(Nature);
    check(Ret != nullptr) return *Ret;
}

void UDefaultStatBlock::Initialize(const TScriptInterface<IPokemon> &NewOwner, const FPokemonDTO &DTO) {
    Owner = NewOwner;
    Level = DTO.Level;

    auto &GrowthRate = UPokemonSubsystem::GetInstance().GetGrowthRate(Owner->GetSpecies().GrowthRate);
    Exp = FMath::Max(GrowthRate.ExpForLevel(Level), DTO.Exp.Get(0));
    Nature = DTO.Nature;

    const auto &DataSubsystem = FDataManager::GetInstance();
    auto &StatTable = DataSubsystem.GetDataTable<FStat>();

    StatTable.ForEach([this, &DTO](const FStat &Stat) {
        auto IV = DTO.IVs.Contains(Stat.ID) ? TOptional<int32>(DTO.IVs[Stat.ID]) : TOptional<int32>();
        int32 EV = DTO.EVs.Contains(Stat.ID) ? DTO.EVs[Stat.ID] : 0;
        switch (Stat.Type) {
            using enum EPokemonStatType;
        case Main:
            Stats.Add(Stat.ID, NewObject<UDefaultMainStatEntry>(this)->Initialize(Stat.ID, IV, EV));
            break;
        case MainBattle:
            Stats.Add(Stat.ID, NewObject<UDefaultMainBattleStatEntry>(this)->Initialize(Stat.ID, IV, EV));
            break;
        case Battle:
            // Skip over this stat as we don't track a value for it
            break;
        }
    });
}

int32 UDefaultStatBlock::GetLevel() const {
    return Level;
}

int32 UDefaultStatBlock::GetExp() const {
    return Exp;
}

int32 UDefaultStatBlock::GetExpForNextLevel() const {
    if (Level == GetMaxLevel())
        return 0;

    return UPokemonSubsystem::GetInstance().GetGrowthRate(Owner->GetSpecies().GrowthRate).ExpForLevel(Level + 1);
}

const FNature &UDefaultStatBlock::GetNature() const {
    if (Nature.IsSet())
        return FindNature(Nature.GetValue());

    auto &DataTable = FDataManager::GetInstance().GetDataTable<FNature>();
    auto NatureRows = DataTable.GetTableRowNames();
    auto Index = static_cast<int32>(Owner->GetPersonalityValue() % NatureRows.Num());
    return *DataTable.GetData(NatureRows[Index]);
}

TScriptInterface<IStatEntry> UDefaultStatBlock::GetStat(FName Stat) const {
    check(Stats.Contains(Stat)) return Stats[Stat];
}

void UDefaultStatBlock::ForEachStat(const TFunctionRef<void(FName, const IStatEntry &)> &Predicate) const {
    for (const auto &[Key, Value] : Stats) {
        Predicate(Key, *Value);
    }
}

void UDefaultStatBlock::CalculateStats(const TMap<FName, int32> &BaseStats) {
    auto &NatureData = GetNature();

    for (const auto &[StatID, Stat] : Stats) {
        check(BaseStats.Contains(StatID)) Stat->RefreshValue(Level, BaseStats[StatID], NatureData);
    }
}
