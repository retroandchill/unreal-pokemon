// "Unreal Pokémon" created by Retro & Chill.


#include "Battle/StatusEffects/StatusEffectTags.h"
#include "DataManager.h"
#include "Battle/Status.h"
#include "Battle/Tags.h"

namespace Pokemon::Battle::StatusEffects {

FLookup::FLookup() {
    auto &DataManager = FDataManager::GetInstance();
    auto& TypeTable = DataManager.GetDataTable<FStatus>();
    TypeTable.ForEach([this](const FStatus& Status) {
        AddDynamicGameplayTag(Tags, StatusEffectTagFormat, Status.ID);
    });
}

FLookup::~FLookup() = default;

FLookup& FLookup::GetInstance() {
    static FLookup Lookup;
    return Lookup;
}

}