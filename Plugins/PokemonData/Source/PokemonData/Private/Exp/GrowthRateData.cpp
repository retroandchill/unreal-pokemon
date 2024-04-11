// "Unreal Pokémon" created by Retro & Chill.
#include "Exp/GrowthRateData.h"

#include "DataManager.h"

TArray<FName> UGrowthRateHelper::GetGrowthRateNames() {
    return FDataManager::GetInstance().GetDataTable<FGrowthRateData>().GetTableRowNames();
}
