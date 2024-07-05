// "Unreal Pokémon" created by Retro & Chill.


#include "Battle/Stats/StatTags.h"
#include "DataManager.h"
#include "Battle/Tags.h"
#include "Species/Stat.h"

namespace Pokemon::Battle::Stats {

FLookup::FLookup() {
    auto &DataManager = FDataManager::GetInstance();
    auto& TypeTable = DataManager.GetDataTable<FStat>();
    TypeTable.ForEach([this](const FStat& Stat) {
        if (Stat.Type == EPokemonStatType::Main) {
            return;
        }
        
        AddDynamicGameplayTag(IgnorePositiveTags, IgnorePositiveStatsFormat, Stat.ID);
        AddDynamicGameplayTag(IgnoreNegativeTags, IgnoreNegativeStatsFormat, Stat.ID);
    });
}

FLookup::~FLookup() = default;

FLookup & FLookup::Get() {
    static FLookup Lookup;
    return Lookup;
}

}