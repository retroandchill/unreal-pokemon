// "Unreal Pokémon" created by Retro & Chill.

#include "Battle/StatusEffects/StatusEffectTags.h"
#include "Battle/Status.h"
#include "Battle/Tags.h"
#include "DataManager.h"
#include "Ranges/Algorithm/ForEach.h"

namespace Pokemon::Battle::StatusEffects {

    FLookup::FLookup() {
        auto &DataManager = FDataManager::GetInstance();
        auto &TypeTable = DataManager.GetDataTable<FStatus>();
        TypeTable.GetAllRows() | UE::Ranges::ForEach([this](const FStatus &Status) {
            AddDynamicGameplayTag(Tags, StatusEffectTagFormat, Status.ID);
            AddDynamicGameplayTag(ImmunityTags, StatusEffectImmunityTagFormat, Status.ID);
        });
    }

    FLookup::~FLookup() = default;

    FLookup &FLookup::GetInstance() {
        static FLookup Lookup;
        return Lookup;
    }

} // namespace Pokemon::Battle::StatusEffects