// "Unreal Pokémon" created by Retro & Chill.
#include "Species/Stat.h"
#include "Algo/RemoveIf.h"
#include "DataManager.h"

TArray<FName> UStatHelper::GetStatNames() {
    return FDataManager::GetInstance().GetDataTable<FStat>().GetTableRowNames();
}

TArray<FName> UStatHelper::GetMainStatNames() {
    auto &StatTable = FDataManager::GetInstance().GetDataTable<FStat>();
    auto Stats = StatTable.GetTableRowNames();
    int32 NewSize = Algo::RemoveIf(
        Stats, [&StatTable](FName Stat) { return StatTable.GetData(Stat)->Type == EPokemonStatType::Battle; });
    Stats.SetNum(NewSize);
    return Stats;
}
