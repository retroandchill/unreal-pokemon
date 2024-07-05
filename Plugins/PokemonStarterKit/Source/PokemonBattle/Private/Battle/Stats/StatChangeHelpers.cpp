// "Unreal Pokémon" created by Retro & Chill.


#include "Battle/Stats/StatChangeHelpers.h"
#include "DataManager.h"
#include "Battle/Battlers/Battler.h"
#include "Battle/Battlers/BattlerAbilityComponent.h"
#include "Battle/Stats/StatTags.h"
#include "Settings/BaseSettings.h"
#include "Species/Stat.h"

FText UStatChangeHelpers::ExtractStatNameFromTag(FGameplayTag Tag) {
    static auto &Lookup = Pokemon::Battle::Stats::FLookup::Get();
    return Lookup.FindStatNameFromGameplayCueTag(Tag);
}

int32 UStatChangeHelpers::GetStatStageValue(const TScriptInterface<IBattler> &Battler, FName Stat) {
    static auto& StatTable = FDataManager::GetInstance().GetInstance().GetDataTable<FStat>();
    auto StatData = StatTable.GetData(Stat);
    check(StatData != nullptr)
    check(StatData->StagesAttribute.IsValid())
    auto Stages = Battler->GetAbilityComponent()->GetNumericAttribute(StatData->StagesAttribute);
    return FMath::RoundToInt32(Stages);
}

bool UStatChangeHelpers::StatStageAtMax(const TScriptInterface<IBattler> &Battler, FName Stat) {
    static auto &StatInfo = Pokemon::FBaseSettings::Get().GetStatStages();
    return GetStatStageValue(Battler, Stat) >= StatInfo.Num();
}

bool UStatChangeHelpers::StatStageAtMin(const TScriptInterface<IBattler> &Battler, FName Stat) {
    static auto &StatInfo = Pokemon::FBaseSettings::Get().GetStatStages();
    return GetStatStageValue(Battler, Stat) <= -StatInfo.Num();
}