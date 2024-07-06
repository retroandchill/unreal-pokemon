// "Unreal Pokémon" created by Retro & Chill.
#include "Species/Stat.h"
#include "Algo/RemoveIf.h"
#include "DataManager.h"
#include "RangeHelpers.h"
#include <range/v3/view/filter.hpp>
#include <range/v3/view/transform.hpp>

TArray<FName> UStatHelper::GetStatNames() {
    return FDataManager::GetInstance().GetDataTable<FStat>().GetTableRowNames();
}

TArray<FName> UStatHelper::GetMainStatNames() {
    auto &StatTable = FDataManager::GetInstance().GetDataTable<FStat>();
    auto Stats = StatTable.GetAllRows();
    return RangeHelpers::CreateRange(Stats) |
           ranges::views::filter([](const FStat *Stat) { return Stat->Type != EPokemonStatType::Battle; }) |
           ranges::views::transform([](const FStat *Stat) { return Stat->ID; }) | RangeHelpers::TToArray<FName>();
}

TArray<FName> UStatHelper::GetBattleStatNames() {
    auto &StatTable = FDataManager::GetInstance().GetDataTable<FStat>();
    auto Stats = StatTable.GetAllRows();
    return RangeHelpers::CreateRange(Stats) |
           ranges::views::filter([](const FStat *Stat) { return Stat->Type != EPokemonStatType::Main; }) |
           ranges::views::transform([](const FStat *Stat) { return Stat->ID; }) | RangeHelpers::TToArray<FName>();
}

TArray<FName> UStatHelper::GetMainBattleStatNames() {
    auto &StatTable = FDataManager::GetInstance().GetDataTable<FStat>();
    auto Stats = StatTable.GetAllRows();
    return RangeHelpers::CreateRange(Stats) |
           ranges::views::filter([](const FStat *Stat) { return Stat->Type == EPokemonStatType::MainBattle; }) |
           ranges::views::transform([](const FStat *Stat) { return Stat->ID; }) | RangeHelpers::TToArray<FName>();
}
