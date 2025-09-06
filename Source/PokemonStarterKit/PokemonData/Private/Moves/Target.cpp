// "Unreal Pokémon" created by Retro & Chill.
#include "Moves/Target.h"
#include "DataManager.h"

TArray<FName> UTargetHelper::GetTargetNames()
{
    return FDataManager::GetInstance().GetDataTable<FMoveTarget>().GetTableRowNames();
}
