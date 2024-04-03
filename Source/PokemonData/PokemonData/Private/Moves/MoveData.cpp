// "Unreal Pokémon" created by Retro & Chill.
#include "Moves/MoveData.h"

#include "DataManager.h"

TArray<FName> UMoveHelper::GetMoveNames() {
	return FDataManager::GetInstance().GetDataTable<FMoveData>().GetTableRowNames();
}
