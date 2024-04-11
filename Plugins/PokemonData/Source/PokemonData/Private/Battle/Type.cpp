// "Unreal Pokémon" created by Retro & Chill.
#include "Battle/Type.h"

#include "DataManager.h"

TArray<FName> UTypeHelper::GetTypeNames() {
    return FDataManager::GetInstance().GetDataTable<FType>().GetTableRowNames();
}
