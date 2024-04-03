// "Unreal Pokémon" created by Retro & Chill.


#include "Moves/DefaultMove.h"

#include "Asserts.h"
#include "DataManager.h"
#include "Moves/MoveData.h"

TScriptInterface<IMove> UDefaultMove::Initialize(FName MoveID) {
	ID = MoveID;
	CurrentPP = GetMoveData().TotalPP;
	return this;
}

const FMoveData& UDefaultMove::GetMoveData() const {
	auto Move = FDataManager::GetInstance().GetDataTable<FMoveData>().GetData(ID);
	ASSERT(Move != nullptr);
	return *Move;
}

int32 UDefaultMove::GetCurrentPP() const {
	return CurrentPP;
}

int32 UDefaultMove::GetTotalPP() const {
	// TODO: Add support for PP ups
	return GetMoveData().TotalPP;
}

bool UDefaultMove::Equals(const TScriptInterface<IMove>& Other) const {
	return Other.GetObject()->GetClass() == StaticClass() ? *this == static_cast<const UDefaultMove&>(*Other) : false;
}

bool UDefaultMove::operator==(const UDefaultMove& Other) const {
	return ID == Other.ID && CurrentPP == Other.CurrentPP;
}
