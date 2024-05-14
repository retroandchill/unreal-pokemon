// "Unreal Pokémon" created by Retro & Chill.
#include "Species/Stat.h"
#include "Algo/RemoveIf.h"
#include "DataManager.h"
#include "Mainpulation/RangeHelpers.h"

TArray<FName> UStatHelper::GetStatNames() {
    return FDataManager::GetInstance().GetDataTable<FStat>().GetTableRowNames();
}

TArray<FName> UStatHelper::GetMainStatNames() {
    auto &StatTable = FDataManager::GetInstance().GetDataTable<FStat>();
    auto Stats = StatTable.GetAllRows();
    return RangeHelpers::CreateRange(Stats) |
           std::views::filter([](const FStat *Stat) { return Stat->Type != EPokemonStatType::Battle; }) |
           std::views::transform([](const FStat *Stat) { return Stat->ID; }) | RangeHelpers::TToArray<FName>();
}
