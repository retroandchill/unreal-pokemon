// "Unreal Pokémon" created by Retro & Chill.
#include "Species/BodyColor.h"
#include "DataManager.h"

TArray<FName> UBodyColorHelper::GetBodyColorNames() {
    return FDataManager::GetInstance().GetDataTable<FBodyColor>().GetTableRowNames();
}
