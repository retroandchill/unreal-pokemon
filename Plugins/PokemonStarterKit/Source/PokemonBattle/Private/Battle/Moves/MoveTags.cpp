// "Unreal Pokémon" created by Retro & Chill.

#include "Battle/Moves/MoveTags.h"
#include "Battle/Tags.h"
#include "DataManager.h"
#include "Moves/MoveData.h"

namespace Pokemon::Battle::Moves {

    const UE_DEFINE_GAMEPLAY_TAG(UsingMove, "Battle.UsingMove");
    const UE_DEFINE_GAMEPLAY_TAG(TypeDetermined, "Battle.Moves.TypeDetermined");
    const UE_DEFINE_GAMEPLAY_TAG(UsingMove_Physical, "Battle.UsingMove.Category.Physical");
    const UE_DEFINE_GAMEPLAY_TAG(UsingMove_Special, "Battle.UsingMove.Category.Special");
    const UE_DEFINE_GAMEPLAY_TAG(UsingMove_Status, "Battle.UsingMove.Category.Status");
    const UE_DEFINE_GAMEPLAY_TAG(MoveTarget, "Battle.MoveTarget");
    const UE_DEFINE_GAMEPLAY_TAG(MoveTarget_Unaffected, "Battle.MoveTarget.Unaffected");
    const UE_DEFINE_GAMEPLAY_TAG(MoveTarget_Unaffected_Missed, "Battle.MoveTarget.Unaffected.Missed");
    const UE_DEFINE_GAMEPLAY_TAG(MoveTarget_Unaffected_Failed, "Battle.MoveTarget.Unaffected.Failed");
    const UE_DEFINE_GAMEPLAY_TAG(MoveTarget_Unaffected_NoEffect, "Battle.MoveTarget.Unaffected.NoEffect");
    const UE_DEFINE_GAMEPLAY_TAG(MoveTarget_NoDamage, "Battle.MoveTarget.NoDamage");
    const UE_DEFINE_GAMEPLAY_TAG(MoveTarget_CriticalHit, "Battle.MoveTarget.CriticalHit");
    const UE_DEFINE_GAMEPLAY_TAG(MoveTarget_Physical, "Battle.MoveTarget.Category.Physical");
    const UE_DEFINE_GAMEPLAY_TAG(MoveTarget_Special, "Battle.MoveTarget.Category.Special");
    const UE_DEFINE_GAMEPLAY_TAG(MoveTarget_Status, "Battle.MoveTarget.Category.Status");
    const DEFINE_TARGETED_EVENT_TYPE(SuccessCheckAgainstTarget, "Battle.Moves.Checks.SuccessCheckAgainstTarget");
    const DEFINE_TARGETED_EVENT_TYPE(HitCheckEvents, "Battle.Moves.AccuracyCheck.Scope");
    const DEFINE_TARGETED_EVENT_TYPE(CriticalHitRateModEvents, "Battle.Moves.CriticalHits.Scope");
    const DEFINE_TARGETED_EVENT_TYPE(DamageModificationEvents, "Battle.Moves.Damage.Scope");
    const UE_DEFINE_GAMEPLAY_TAG(DamageDealtValue, "Battle.Moves.Damage.Value");
    const DEFINE_TARGETED_EVENT_TYPE(AdditionalEffectChanceEvents, "Battle.Moves.Effects.Additional.Scope");

    const UE_DEFINE_GAMEPLAY_TAG(TwoTurnAttack, "Battle.Moves.TwoTurnAttack");

    const FNativeGameplayTag &GetUserCategoryTag(EMoveDamageCategory Category) {
        using enum EMoveDamageCategory;
        switch (Category) {
        case Physical:
            return UsingMove_Physical;
        case Special:
            return UsingMove_Special;
        default: // Status
            return UsingMove_Status;
        }
    }

    const FNativeGameplayTag &GetTargetCategoryTag(EMoveDamageCategory Category) {
        using enum EMoveDamageCategory;
        switch (Category) {
        case Physical:
            return MoveTarget_Physical;
        case Special:
            return MoveTarget_Special;
        default: // Status
            return MoveTarget_Status;
        }
    }

    FLookup::FLookup() {
        auto &DataManager = FDataManager::GetInstance();
        auto &TypeTable = DataManager.GetDataTable<FMoveData>();
        TypeTable.ForEach([this](const FMoveData &Move) {
            for (auto Tag : Move.Tags) {
                if (Tags.Contains(Tag)) {
                    continue;
                }

                AddDynamicGameplayTag(Tags, MoveTagFormat, Tag);
            }
        });
    }

    FLookup::~FLookup() = default;

    FLookup &FLookup::GetInstance() {
        static FLookup Lookup;
        return Lookup;
    }
} // namespace Pokemon::Battle::Moves