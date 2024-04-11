// "Unreal Pokémon" created by Retro & Chill.
#include "Species/EggGroup.h"

#include "DataManager.h"

TArray<FName> UEggGroupHelper::GetEggGroupNames() {
    return FDataManager::GetInstance().GetDataTable<FEggGroup>().GetTableRowNames();
}
