// "Unreal Pokémon" created by Retro & Chill.

#include "Battle/Stats/StatTags.h"
#include "Battle/Tags.h"
#include "DataManager.h"
#include "Ranges/Algorithm/ForEach.h"
#include "Species/Stat.h"

namespace Pokemon::Battle::Stats {

    const UE_DEFINE_GAMEPLAY_TAG(StagesTag, "Battle.Battler.Stats.Stages")

        FLookup::FLookup() {
        auto &DataManager = FDataManager::GetInstance();
        auto &TypeTable = DataManager.GetDataTable<FStat>();
        TypeTable.GetAllRows() | UE::Ranges::ForEach([this](const FStat &Stat) {
            if (Stat.Type == EPokemonStatType::Main) {
                return;
            }

            AddDynamicGameplayTag(IgnorePositiveTags, IgnorePositiveStatsFormat, Stat.ID);
            AddDynamicGameplayTag(IgnoreNegativeTags, IgnoreNegativeStatsFormat, Stat.ID);
            AddDynamicGameplayTag(GameplayCueTags, GameplayCueTagFormat, Stat.ID);
        });
    }

    FLookup::~FLookup() = default;

    FLookup &FLookup::Get() {
        static FLookup Lookup;
        return Lookup;
    }

    FText FLookup::FindStatNameFromGameplayCueTag(const FGameplayTag &Tag) const {
        static auto &TypeTable = FDataManager::GetInstance().GetDataTable<FStat>();
        for (auto &[ID, NativeTag] : GameplayCueTags) {
            if (Tag == *NativeTag) {
                auto Stat = TypeTable.GetData(ID);
                check(Stat != nullptr)
                return Stat->RealName;
            }
        }

        return FText::FromStringView(TEXT("???"));
    }

} // namespace Pokemon::Battle::Stats