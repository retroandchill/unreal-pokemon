// "Unreal Pokémon" created by Retro & Chill.
#include "Species/Ability.h"
#include "DataManager.h"

TArray<FName> UAbilityHelper::GetAbilityNames()
{
    return FDataManager::GetInstance().GetDataTable<FAbility>().GetTableRowNames();
}

FName UAbilityHelper::ConvertAbilityHandleHandleToName(const FAbilityHandle &Struct)
{
    return Struct;
}

FAbilityHandle UAbilityHelper::ConvertNameToAbilityHandle(FName Name)
{
    return Name;
}
