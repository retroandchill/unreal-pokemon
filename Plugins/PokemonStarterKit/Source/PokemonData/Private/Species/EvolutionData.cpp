// "Unreal Pokémon" created by Retro & Chill.
#include "Species/EvolutionData.h"
#include "DataManager.h"

TArray<FName> UEvolutionHelper::GetEvolutionNames() {
    return FDataManager::GetInstance().GetDataTable<FEvolutionData>().GetTableRowNames();
}
