// "Unreal Pokémon" created by Retro & Chill.


#include "Battle/Stats/StatChangeHelpers.h"
#include "DataManager.h"
#include "Battle/Battlers/Battler.h"
#include "Battle/Battlers/BattlerAbilityComponent.h"
#include "Battle/Stats/StatChangeCalculation.h"
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

int32 UStatChangeHelpers::ChangeBattlerStatStages(const TScriptInterface<IBattler> &Battler, FName Stat, int32 Stages) {
    static auto& StatTable = FDataManager::GetInstance().GetDataTable<FStat>();
    auto StatData = StatTable.GetData(Stat);
    check(StatData != nullptr)
    check(StatData->StagesAttribute.IsValid())

    auto AbilityComponent = Battler->GetAbilityComponent();
    int32 CurrentStages = FMath::FloorToInt32(AbilityComponent->GetNumericAttribute(StatData->StagesAttribute));
    int32 ActualChange = CurrentStages + Stages;

    static auto &StatInfo = Pokemon::FBaseSettings::Get().GetStatStages();
    int32 Bound = StatInfo.Num();
    ActualChange = FMath::Clamp(ActualChange, -Bound, Bound) - CurrentStages;

    auto StatChangeEffect = NewObject<UGameplayEffect>();
    StatChangeEffect->DurationPolicy = EGameplayEffectDurationType::Instant;
    
    auto& Modifier = StatChangeEffect->Modifiers.Emplace_GetRef();
    Modifier.Attribute = StatData->StagesAttribute;
    Modifier.ModifierOp = EGameplayModOp::Additive;

    FCustomCalculationBasedFloat Calculation;
    Calculation.CalculationClassMagnitude = UStatChangeCalculation::StaticClass();
    Modifier.ModifierMagnitude = FGameplayEffectModifierMagnitude(Calculation);

    static auto& Lookup = Pokemon::Battle::Stats::FLookup::Get();
    StatChangeEffect->GameplayCues.Emplace(Lookup.GetGameplayCueTag(Stat), 0.f, 0.f);

    auto Context = AbilityComponent->MakeEffectContext();
    FGameplayEffectSpec NewSpec(StatChangeEffect, Context, 1);
    NewSpec.SetSetByCallerMagnitude(Pokemon::Battle::Stats::StagesTag, static_cast<float>(ActualChange));
    AbilityComponent->ApplyGameplayEffectSpecToSelf(NewSpec);
    return ActualChange;
}