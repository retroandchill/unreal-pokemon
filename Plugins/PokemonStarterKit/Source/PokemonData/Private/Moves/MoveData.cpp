// "Unreal Pokémon" created by Retro & Chill.
#include "Moves/MoveData.h"
#include "DataManager.h"

TArray<FName> UMoveHelper::GetMoveNames() {
    return FDataManager::GetInstance().GetDataTable<FMoveData>().GetTableRowNames();
}

FName UMoveHelper::ConvertMoveHandleHandleToName(const FMoveHandle &Struct) {
    return Struct;
}

FMoveHandle UMoveHelper::ConvertNameToMoveHandle(FName Name) {
    return Name;
}
