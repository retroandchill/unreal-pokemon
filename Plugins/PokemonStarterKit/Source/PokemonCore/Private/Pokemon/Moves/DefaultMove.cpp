// "Unreal Pokémon" created by Retro & Chill.

#include "Pokemon/Moves/DefaultMove.h"
#include "DataManager.h"
#include "Moves/MoveData.h"

TScriptInterface<IMove> UDefaultMove::Initialize(FName MoveID) {
    ID = MoveID;
    CurrentPP = GetMoveData().TotalPP;
    return this;
}

const FMoveData &UDefaultMove::GetMoveData() const {
    auto Move = FDataManager::GetInstance().GetDataTable<FMoveData>().GetData(ID);
    check(Move != nullptr)
    return *Move;
}

EMoveDamageCategory UDefaultMove::GetDamageCategory() const {
    return GetMoveData().Category;
}

int32 UDefaultMove::GetBasePower() const {
    return GetMoveData().Power;
}

int32 UDefaultMove::GetCurrentPP() const {
    return CurrentPP;
}

int32 UDefaultMove::GetTotalPP() const {
    // TODO: Add support for PP ups
    return GetMoveData().TotalPP;
}
