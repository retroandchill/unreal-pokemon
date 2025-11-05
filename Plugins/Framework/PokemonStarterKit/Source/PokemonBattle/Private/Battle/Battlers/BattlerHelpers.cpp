// "Unreal Pokémon" created by Retro & Chill.

#include "Battle/Battlers/BattlerHelpers.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "AbilitySystemComponent.h"
#include "Battle/Attributes/PokemonCoreAttributeSet.h"
#include "Battle/Battle.h"
#include "Battle/Battlers/BattlerAbilityComponent.h"
#include "Battle/BattleSide.h"
#include "Battle/Display/BattleHUD.h"
#include "Battle/Effects/BattleEffectUtilities.h"
#include "Battle/Settings/BattleMessageSettings.h"
#include "Battle/Stats/StatChangeCalculation.h"
#include "RetroLib/Ranges/Algorithm/To.h"
#include "Utilities/PokemonCoroutineDispatcher.h"

float UBattlerHelpers::GetBattlerStat(const FGameplayAbilityActorInfo &ActorInfo, FGameplayAttribute Attribute,
                                      bool &bFound) {
    return ActorInfo.AbilitySystemComponent->GetGameplayAttributeValue(Attribute, bFound);
}

EStatusEffectStatus UBattlerHelpers::GetStatusEffect(const TScriptInterface<IBattler> &Battler,
                                                     FStatusEffectInfo &StatusEffect) {
    auto StatusEffectInfo = Battler->GetStatusEffect();
    if (!StatusEffectInfo.IsSet()) {
        return EStatusEffectStatus::NoStatusEffect;
    }

    StatusEffect = *StatusEffectInfo;
    return EStatusEffectStatus::HasStatusEffect;
}

UE5Coro::TCoroutine<> UBattlerHelpers::GainExpOnFaint(UE5Coro::TLatentContext<const UObject> Context,
                                                      const TArray<TScriptInterface<IBattler>> &FainedBattlers) {
    auto ValidBattlers = FainedBattlers | Retro::Ranges::Views::Filter([](const TScriptInterface<IBattler> &Battler) {
                             auto &OwningSide = Battler->GetOwningSide();
                             return OwningSide == OwningSide->GetOwningBattle()->GetOpposingSide();
                         }) |
                         Retro::Ranges::To<TArray>();
    if (ValidBattlers.IsEmpty()) {
        co_return;
    }

    auto GainInfos = co_await ValidBattlers[0]->GiveExpToParticipants();
    for (int32 i = 1; i < ValidBattlers.Num(); i++) {
        auto AdditionalInfos = co_await ValidBattlers[i]->GiveExpToParticipants();
        check(GainInfos.Num() == AdditionalInfos.Num())

        for (int32 j = 0; j < GainInfos.Num(); j++) {
            auto &MainInfo = GainInfos[j];
            auto &CopiedInfo = AdditionalInfos[j];

            check(MainInfo.GainingBattler == CopiedInfo.GainingBattler)
            MainInfo.Amount += CopiedInfo.Amount;
            MainInfo.StatChanges += CopiedInfo.StatChanges;
        }
    }

    for (auto &Info : GainInfos) {
        if (Info.StatChanges.LevelChange.Diff() > 0) {
            Info.GainingBattler->RefreshStats();
        }
    }

    auto HUD = ValidBattlers[0]->GetOwningSide()->GetOwningBattle()->GetBattleHUD();
    if (!Retro::IsValidPtr(HUD)) {
        co_return;
    }

    co_await HUD->DisplayExpForGain(Context, std::move(GainInfos));
}

UE5Coro::TCoroutine<bool> UBattlerHelpers::ApplyHPRecoveryEffect(const TScriptInterface<IBattler> &Battler,
                                                                 int32 Amount, const UGameplayAbility *Ability,
                                                                 bool bShowFailureMessage, FForceLatentCoroutine) {
    auto AbilityComponent = Battler->GetAbilityComponent();
    auto Attributes = AbilityComponent->GetCoreAttributes();
    float CurrentHP = Attributes->GetHP();
    float MaxHP = Attributes->GetMaxHP();

    auto &Dispatcher = IPokemonCoroutineDispatcher::Get(Battler);
    auto Settings = GetDefault<UBattleMessageSettings>();
    if (MaxHP <= CurrentHP) {
        if (bShowFailureMessage) {
            co_await Dispatcher.DisplayMessage(
                FText::FormatNamed(Settings->NoEffectMessage, "Pkmn", Battler->GetNickname()));
        }
        co_return false;
    }

    auto RecoveryEffect = NewObject<UGameplayEffect>();
    RecoveryEffect->DurationPolicy = EGameplayEffectDurationType::Instant;

    auto &Modifier = RecoveryEffect->Modifiers.Emplace_GetRef();
    Modifier.Attribute = UPokemonCoreAttributeSet::GetHPAttribute();
    Modifier.ModifierOp = EGameplayModOp::Additive;
    Modifier.ModifierMagnitude = FGameplayEffectModifierMagnitude(static_cast<float>(Amount));

    FGameplayEffectContextHandle Context;
    if (Ability != nullptr) {
        Context = Ability->MakeEffectContext(Ability->GetCurrentAbilitySpecHandle(), Ability->GetCurrentActorInfo());
    } else {
        Context = AbilityComponent->MakeEffectContext();
    }
    AbilityComponent->ApplyGameplayEffectToSelf(RecoveryEffect, 1, Context);

    auto &BattleHUD = Battler->GetOwningSide()->GetOwningBattle()->GetBattleHUD();
    co_await BattleHUD->AnimateHPChange(Battler);
    co_await Dispatcher.DisplayMessage(FText::FormatNamed(Settings->HPRestoredMessage, "Pkmn", Battler->GetNickname()));

    co_return true;
}