// "Unreal Pokémon" created by Retro & Chill.
#include "Species/Ability.h"

#include "DataManager.h"

TArray<FName> UAbilityHelper::GetAbilityNames() {
    return FDataManager::GetInstance().GetDataTable<FAbility>().GetTableRowNames();
}
