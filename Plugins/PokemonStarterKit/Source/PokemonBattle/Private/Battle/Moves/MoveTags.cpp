// "Unreal Pokémon" created by Retro & Chill.


#include "Battle/Moves/MoveTags.h"
#include "DataManager.h"
#include "Battle/Tags.h"
#include "Moves/MoveData.h"

namespace Pokemon::Battle::Moves {

UE_DEFINE_GAMEPLAY_TAG(UsingMove, "Battle.UsingMove");
DEFINE_TARGETED_EVENT_TYPE(SuccessCheckAgainstTarget, "Battle.Moves.Checks.SuccessCheckAgainstTarget");

UE_DEFINE_GAMEPLAY_TAG(TwoTurnAttack, "Battle.Moves.TwoTurnAttack");

FLookup::FLookup() {
    auto &DataManager = FDataManager::GetInstance();
    auto& TypeTable = DataManager.GetDataTable<FMoveData>();
    TypeTable.ForEach([this](const FMoveData& Move) {
        for (auto Tag : Move.Tags) {
            if (Tags.Contains(Tag)) {
                continue;
            }

            AddDynamicGameplayTag(Tags, MoveTagFormat, Tag);
        }
    });
}

FLookup::~FLookup() = default;

FLookup & FLookup::GetInstance() {
    static FLookup Lookup;
    return Lookup;
}
}