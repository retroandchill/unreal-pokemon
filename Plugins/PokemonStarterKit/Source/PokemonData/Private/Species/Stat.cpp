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

FName UStatHelper::ConvertStatHandleToName(const FStatHandle &Struct) {
    return Struct;
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

FStatHandle UStatHelper::ConvertNameToItemHandle(FName Name) {
    return Name;
}

FName UStatHelper::ConvertMainStatHandleToName(const FMainStatHandle &Struct) {
    return Struct;
}

FMainStatHandle UStatHelper::ConvertNameToMainStatHandle(FName Name) {
    return Name;
}

FName UStatHelper::ConvertBattleStatHandleToName(const FBattleStatHandle &Struct) {
    return Struct;
}

FBattleStatHandle UStatHelper::ConvertNameToBattleStatHandle(FName Name) {
    return Name;
}

FName UStatHelper::ConvertItemMainBattleStatHandleToName(const FMainBattleStatHandle &Struct) {
    return Struct;
}

FMainBattleStatHandle UStatHelper::ConverMainBattleStatHandle(FName Name) {
    return Name;
}