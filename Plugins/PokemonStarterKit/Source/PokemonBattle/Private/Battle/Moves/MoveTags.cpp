// "Unreal Pokémon" created by Retro & Chill.


#include "Battle/Moves/MoveTags.h"
#include "DataManager.h"
#include "Battle/Tags.h"
#include "Moves/MoveData.h"

namespace Pokemon::Battle::Moves {

UE_DEFINE_GAMEPLAY_TAG(UsingMove, "Battle.UsingMove");
UE_DEFINE_GAMEPLAY_TAG(MoveTarget, "Battle.MoveTarget");
UE_DEFINE_GAMEPLAY_TAG(MoveTarget_Unaffected, "Battle.MoveTarget.Unaffected");
UE_DEFINE_GAMEPLAY_TAG(MoveTarget_Unaffected_Missed, "Battle.MoveTarget.Unaffected.Missed");
UE_DEFINE_GAMEPLAY_TAG(MoveTarget_Unaffected_Failed, "Battle.MoveTarget.Unaffected.Failed");
UE_DEFINE_GAMEPLAY_TAG(MoveTarget_NoDamage, "Battle.MoveTarget.NoDamage");
UE_DEFINE_GAMEPLAY_TAG(MoveTarget_CriticalHit, "Battle.MoveTarget.CriticalHit");
DEFINE_TARGETED_EVENT_TYPE(SuccessCheckAgainstTarget, "Battle.Moves.Checks.SuccessCheckAgainstTarget");
DEFINE_TARGETED_EVENT_TYPE(HitCheckEvents, "Battle.Moves.AccuracyCheck.Scope");
DEFINE_TARGETED_EVENT_TYPE(CriticalHitRateModEvents, "Battle.Moves.CriticalHits.Scope");
DEFINE_TARGETED_EVENT_TYPE(DamageModificationEvents, "Battle.Moves.Damage.Scope");

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