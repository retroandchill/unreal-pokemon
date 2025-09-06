// "Unreal Pokémon" created by Retro & Chill.

#include "Battle/Stats/StatChangeHelpers.h"
#include "Battle/Attributes/StatStagesAttributeSet.h"
#include "Battle/Battlers/Battler.h"
#include "Battle/Battlers/BattlerAbilityComponent.h"
#include "Battle/Settings/BattleMessageSettings.h"
#include "Battle/Settings/PokemonBattleSettings.h"
#include "Battle/Stats/StatChangeCalculation.h"
#include "Battle/Stats/StatTags.h"
#include "DataManager.h"
#include "Species/Stat.h"
#include "Utilities/PokemonCoroutineDispatcher.h"

UE5Coro::TCoroutine<bool> UStatChangeHelpers::CanRaiseStat(UE5Coro::TLatentContext<const UObject> Context,
                                                           const TScriptInterface<IBattler> &Battler,
                                                           FMainBattleStatHandle Stat, bool bShowMessages,
                                                           bool bIgnoreInversion)
{
    if (!bIgnoreInversion && Battler->GetAbilityComponent()->GetStatStages()->GetStatStageMultiplier() < 0)
    {
        co_return co_await CanLowerStat(Context, Battler, Stat, bShowMessages, true);
    }

    if (!StatStageAtMax(Battler, Stat))
    {
        co_return true;
    }

    if (bShowMessages)
    {
        auto Settings = GetDefault<UBattleMessageSettings>();
        co_await IPokemonCoroutineDispatcher::Get(Context.Target)
            .DisplayMessage(FText::FormatNamed(Settings->MaxStatMessage, "Pkmn", Battler->GetNickname(), "Stat",
                                               FDataManager::GetInstance().GetDataChecked(Stat).RealName));
    }

    co_return false;
}

UE5Coro::TCoroutine<bool> UStatChangeHelpers::CanLowerStat(UE5Coro::TLatentContext<const UObject> Context,
                                                           const TScriptInterface<IBattler> &Battler,
                                                           FMainBattleStatHandle Stat, bool bShowMessages,
                                                           bool bIgnoreInversion)
{
    if (!bIgnoreInversion && Battler->GetAbilityComponent()->GetStatStages()->GetStatStageMultiplier() < 0)
    {
        co_return co_await CanRaiseStat(Context, Battler, Stat, bShowMessages, true);
    }

    if (!StatStageAtMin(Battler, Stat))
    {
        co_return true;
    }

    if (bShowMessages)
    {
        auto Settings = GetDefault<UBattleMessageSettings>();
        co_await IPokemonCoroutineDispatcher::Get(Context.Target)
            .DisplayMessage(FText::FormatNamed(Settings->MinStatMessage, "Pkmn", Battler->GetNickname(), "Stat",
                                               FDataManager::GetInstance().GetDataChecked(Stat).RealName));
    }

    co_return false;
}

int32 UStatChangeHelpers::GetStatStageValue(const TScriptInterface<IBattler> &Battler, FMainBattleStatHandle Stat)
{
    static auto &StatTable = FDataManager::GetInstance().GetInstance().GetDataTable<FStat>();
    auto StatData = StatTable.GetData(Stat);
    check(StatData != nullptr)
    check(StatData->StagesAttribute.IsValid())
    auto Stages = Battler->GetAbilityComponent()->GetNumericAttribute(StatData->StagesAttribute);
    return FMath::RoundToInt32(Stages);
}

bool UStatChangeHelpers::StatStageAtMax(const TScriptInterface<IBattler> &Battler, FMainBattleStatHandle Stat)
{
    static auto &StatInfo = GetDefault<UPokemonBattleSettings>()->StatStages;
    return GetStatStageValue(Battler, Stat) >= StatInfo.Num();
}

bool UStatChangeHelpers::StatStageAtMin(const TScriptInterface<IBattler> &Battler, FMainBattleStatHandle Stat)
{
    static auto &StatInfo = GetDefault<UPokemonBattleSettings>()->StatStages;
    return GetStatStageValue(Battler, Stat) <= -StatInfo.Num();
}

TOptional<FText> UStatChangeHelpers::GetStatChangeMessage(const TScriptInterface<IBattler> &Battler,
                                                          const FText &StatName, int32 Change)
{
    if (Change == 0)
    {
        return {};
    }

    auto &Settings = GetDefault<UBattleMessageSettings>()->StatChangeMessage;
    auto &ChangeText = Change > 0 ? Settings.IncreaseText : Settings.DecreaseText;

    check(!Settings.Modifiers.IsEmpty())
    int32 ModiferIndex = FMath::Abs(Change) - 1;
    auto &ModifierText =
        Settings.Modifiers.IsValidIndex(ModiferIndex) ? Settings.Modifiers[ModiferIndex] : Settings.Modifiers.Last();

    auto FullChangeText =
        ModifierText.IsEmptyOrWhitespace()
            ? ChangeText
            : FText::FormatNamed(Settings.ModifiedChangeFormat, "Change", ChangeText, "Modifier", ModifierText);

    return FText::FormatNamed(Settings.StatChangeMessage, "Pkmn", Battler->GetNickname(), "Stat", StatName, "Change",
                              std::move(FullChangeText));
}

UE5Coro::TCoroutine<int32> UStatChangeHelpers::ChangeBattlerStatStages(const TScriptInterface<IBattler> &Battler,
                                                                       FName Stat, int32 Stages,
                                                                       UGameplayAbility *Ability)
{
    static auto &StatTable = FDataManager::GetInstance().GetDataTable<FStat>();
    auto StatData = StatTable.GetData(Stat);
    check(StatData != nullptr)
    check(StatData->StagesAttribute.IsValid())

    auto AbilityComponent = Battler->GetAbilityComponent();
    int32 CurrentStages = FMath::FloorToInt32(AbilityComponent->GetNumericAttribute(StatData->StagesAttribute));
    int32 ActualChange = CurrentStages + Stages;

    static auto &StatInfo = GetDefault<UPokemonBattleSettings>()->StatStages;
    int32 Bound = StatInfo.Num();
    ActualChange = FMath::Clamp(ActualChange, -Bound, Bound) - CurrentStages;

    auto StatChangeEffect = NewObject<UGameplayEffect>();
    StatChangeEffect->DurationPolicy = EGameplayEffectDurationType::Instant;

    auto &Modifier = StatChangeEffect->Modifiers.Emplace_GetRef();
    Modifier.Attribute = StatData->StagesAttribute;
    Modifier.ModifierOp = EGameplayModOp::Additive;

    FCustomCalculationBasedFloat Calculation;
    Calculation.CalculationClassMagnitude = UStatChangeCalculation::StaticClass();
    Modifier.ModifierMagnitude = FGameplayEffectModifierMagnitude(Calculation);

    FGameplayEffectContextHandle Context;
    if (Ability != nullptr)
    {
        Context = Ability->MakeEffectContext(Ability->GetCurrentAbilitySpecHandle(), Ability->GetCurrentActorInfo());
    }
    else
    {
        Context = AbilityComponent->MakeEffectContext();
    }
    FGameplayEffectSpec NewSpec(StatChangeEffect, Context, 1);
    NewSpec.SetSetByCallerMagnitude(Pokemon::Battle::Stats::StagesTag, static_cast<float>(ActualChange));
    AbilityComponent->ApplyGameplayEffectSpecToSelf(NewSpec);

    if (auto ChangeMessage = GetStatChangeMessage(Battler, StatData->RealName, ActualChange); ChangeMessage.IsSet())
    {
        co_await IPokemonCoroutineDispatcher::Get(Battler.GetObject()).DisplayMessage(*ChangeMessage);
    }

    co_return ActualChange;
}