// "Unreal Pokémon" created by Retro & Chill.


#include "Moves/DefaultMove.h"

#include "Asserts.h"
#include "DataManager.h"

IMPLEMENT_DERIVED_METATYPE(FDefaultMove)

/**
 * Internal helper function for getting a move's data
 * @param MoveID The ID of the move in question
 * @return The moves data
 */
TRowPointer<FMoveData> FindMoveData(FName MoveID) {
	auto Move = FDataManager::GetInstance().GetDataTable<FMoveData>().GetDataManaged(MoveID);
	ASSERT(Move != nullptr);
	return Move;
}

FDefaultMove::FDefaultMove(FName MoveID) : MoveData(FindMoveData(MoveID)), CurrentPP(MoveData->TotalPP) {
}

const FMoveData& FDefaultMove::GetMoveData() const {
	return *MoveData;
}

int32 FDefaultMove::GetCurrentPP() const {
	return CurrentPP;
}

int32 FDefaultMove::GetTotalPP() const {
	// TODO: Add support for PP ups
	return MoveData->TotalPP;
}

bool FDefaultMove::operator==(const IMove& Other) const {
	return ClassName() == Other.GetClassName() ? *this == static_cast<const FDefaultMove&>(Other) : false;
}

bool FDefaultMove::operator==(const FDefaultMove& Other) const {
	return MoveData == Other.MoveData && CurrentPP == Other.CurrentPP;
}
