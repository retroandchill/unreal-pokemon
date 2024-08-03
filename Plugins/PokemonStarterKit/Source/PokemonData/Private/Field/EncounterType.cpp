// "Unreal Pokémon" created by Retro & Chill.
#include "Field/EncounterType.h"
#include "DataManager.h"

TArray<FName> UEncounterTypeUtils::GetEncounterTypes() {
    return FDataManager::GetInstance().GetDataTable<FEncounterType>().GetTableRowNames();
}