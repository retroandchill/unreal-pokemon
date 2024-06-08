// "Unreal Pokémon" created by Retro & Chill.
#include "Species/Nature.h"
#include "DataManager.h"

TArray<FName> UNatureHelper::GetNatureNames() {
    return FDataManager::GetInstance().GetDataTable<FNature>().GetTableRowNames();
}
