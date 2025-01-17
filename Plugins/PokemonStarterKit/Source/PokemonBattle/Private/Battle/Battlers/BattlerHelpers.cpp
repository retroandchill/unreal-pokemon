// "Unreal Pokémon" created by Retro & Chill.

#include "Battle/Battlers/BattlerHelpers.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "AbilitySystemComponent.h"
#include "Battle/Battle.h"
#include "Battle/BattleSide.h"
#include "Battle/Display/BattleHUD.h"
#include "RetroLib/Ranges/Algorithm/To.h"

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

UE5Coro::TCoroutine<> UBattlerHelpers::GainExpOnFaint(const TArray<TScriptInterface<IBattler>> &FainedBattlers) {
    auto ValidBattlers = FainedBattlers |
        Retro::Ranges::Views::Filter([](const TScriptInterface<IBattler> &Battler) {
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

    co_await HUD->DisplayExpForGain(std::move(GainInfos));
}