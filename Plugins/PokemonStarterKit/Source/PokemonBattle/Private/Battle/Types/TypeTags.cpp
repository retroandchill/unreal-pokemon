// "Unreal Pokémon" created by Retro & Chill.

#include "Battle/Types/TypeTags.h"
#include "Battle/Tags.h"
#include "Battle/Type.h"
#include "DataManager.h"

namespace Pokemon::Battle::Types {

    DEFINE_TARGETED_EVENT_TYPE(SingleTypeModifierEvents, "Battle.Damage.TypeMatchup.Single");
    DEFINE_TARGETED_EVENT_TYPE(FullTypeMatchUpEvents, "Battle.Damage.TypeMatchup.Full");
    UE_DEFINE_GAMEPLAY_TAG(BattlerTypes, "Battle.Battler.Types")

    FLookup::FLookup() {
        auto &DataManager = FDataManager::GetInstance();
        auto &TypeTable = DataManager.GetDataTable<FType>();
        TypeTable.ForEach([this](const FType &Type) {
            // Add the tags for the types you attack from and defend from
            if (!Type.IsPseudoType) {
                AddDynamicGameplayTag(AttackingTags, AttackingTagsFormat, Type.ID);
                AddDynamicGameplayTag(DefendingTags, DefendingTagsFormat, Type.ID);
            }

            AddDynamicGameplayTag(MoveTypeUserTags, MoveTypeUserFormat, Type.ID);
            AddDynamicGameplayTag(MoveTypeTargetTags, MoveTypeTargetFormat, Type.ID);
        });
    }

    FLookup::~FLookup() = default;

    FLookup &FLookup::GetInstance() {
        static FLookup Lookup;
        return Lookup;
    }

} // namespace Pokemon::Battle::Types