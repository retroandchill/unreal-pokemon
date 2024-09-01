// "Unreal Pokémon" created by Retro & Chill.
#include "Species/Stat.h"
#include "Algo/RemoveIf.h"
#include "DataManager.h"
#include "Ranges/Algorithm/ToArray.h"
#include "Ranges/Views/ContainerView.h"
#include "Ranges/Views/Filter.h"
#include "Ranges/Views/Map.h"

TArray<FName> UStatHelper::GetStatNames() {
    return FDataManager::GetInstance().GetDataTable<FStat>().GetTableRowNames();
}

TArray<FName> UStatHelper::GetMainStatNames() {
    auto &StatTable = FDataManager::GetInstance().GetDataTable<FStat>();
    // clang-format off
    return StatTable.GetAllRows() |
           UE::Ranges::Filter([](const FStat &Stat) {
               return Stat.Type != EPokemonStatType::Battle;
           }) |
           UE::Ranges::Map(&FStat::ID) |
           UE::Ranges::ToArray;
    // clang-format on
}

TArray<FName> UStatHelper::GetBattleStatNames() {
    auto &StatTable = FDataManager::GetInstance().GetDataTable<FStat>();
    // clang-format off
    return StatTable.GetAllRows() |
           UE::Ranges::Filter([](const FStat &Stat) {
               return Stat.Type != EPokemonStatType::Main;
           }) |
           UE::Ranges::Map(&FStat::ID) |
           UE::Ranges::ToArray;
    // clang-format on
}

TArray<FName> UStatHelper::GetMainBattleStatNames() {
    auto &StatTable = FDataManager::GetInstance().GetDataTable<FStat>();
    // clang-format off
    return StatTable.GetAllRows() |
           UE::Ranges::Filter([](const FStat &Stat) {
               return Stat.Type == EPokemonStatType::MainBattle;
           }) |
           UE::Ranges::Map(&FStat::ID) |
           UE::Ranges::ToArray;
    // clang-format on
}