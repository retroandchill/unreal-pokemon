// "Unreal Pokémon" created by Retro & Chill.

#include "Battle/StatusEffects/BattleStatusEffectUtils.h"
#include "Battle/Animations/BattleSequencer.h"
#include "Battle/Battlers/Battler.h"
#include "Battle/Battlers/BattlerAbilityComponent.h"
#include "Battle/StatusEffects/StatusEffectLookup.h"
#include "Settings/PokemonMessageSettings.h"
#include "Utilities/PokemonCoroutineDispatcher.h"

UE5Coro::TCoroutine<bool> UBattleStatusEffectUtils::CanStatusEffectBeInflicted(
    const TScriptInterface<IBattler> &Target,
    FName StatusEffectID, const FText& AlreadyAppliedFormat, const FText& HasOtherStatusFormat, FForceLatentCoroutine) {
    auto &StatusEffect = Target->GetStatusEffect();
    if (!StatusEffect.IsSet()) {
        co_return true;
    }

    auto &Dispatcher = IPokemonCoroutineDispatcher::Get(Target.GetObject());
    if (auto &Status = StatusEffect.GetValue(); Status.StatusEffectID == StatusEffectID) {
        co_await Dispatcher.DisplayMessage(FText::FormatNamed(AlreadyAppliedFormat, "Pkmn", Target->GetNickname()));
    } else {
        co_await Dispatcher.DisplayMessage(FText::FormatNamed(HasOtherStatusFormat, "Pkmn", Target->GetNickname()));
    }

    co_return false;

}

UE5Coro::TCoroutine<FActiveGameplayEffectHandle> UBattleStatusEffectUtils::ApplyStatusEffectToBattler(
    const TScriptInterface<IBattler> &Battler, FStatusHandle StatusEffect, FForceLatentCoroutine) {
    FGameplayAbilityTargetDataHandle TargetDataHandle;
    auto &TargetData = TargetDataHandle.Data.Emplace_GetRef(MakeShared<FGameplayAbilityTargetData_ActorArray>());
    TargetData->SetActors({CastChecked<AActor>(Battler.GetObject())});
    auto AbilityComponent = Battler->GetAbilityComponent();
    
    auto StatusEffectClass = Pokemon::Battle::StatusEffects::FindStatusEffect(StatusEffect);
    check(StatusEffectClass.IsSet());
    auto Spec = AbilityComponent->MakeOutgoingSpec(StatusEffectClass.GetValue(), 0,
                                                              AbilityComponent->MakeEffectContext());
    auto Handle = AbilityComponent->ApplyGameplayEffectSpecToSelf(*Spec.Data);
    Battler->InflictStatusEffect(StatusEffect, Handle);

    auto Settings = GetDefault<UPokemonMessageSettings>();
    if (auto MessageFormat = Settings->ObtainedStatusEffectMessages.Find(StatusEffect); MessageFormat != nullptr) {
        auto &Dispatcher = IPokemonCoroutineDispatcher::Get(Battler.GetObject());
        co_await Dispatcher.DisplayMessage(FText::FormatNamed(*MessageFormat, "Pkmn", Battler->GetNickname()));
    }

    co_return Handle;
}

UE5Coro::TCoroutine<bool> UBattleStatusEffectUtils::RemoveStatusEffectFromBattler(
    const TScriptInterface<IBattler> &Target, FForceLatentCoroutine) {
        auto &Status = Target->GetStatusEffect();
        if (!Status.IsSet()) {
            co_return false;
        }

        if (!Target->GetAbilityComponent()->RemoveActiveGameplayEffect(Status->EffectHandle)) {
            co_return false;
        }

        auto Settings = GetDefault<UPokemonMessageSettings>();
        if (auto MessageFormat = Settings->StatusEffectCuredMessages.Find(Status->StatusEffectID); MessageFormat != nullptr) {
            auto &Dispatcher = IPokemonCoroutineDispatcher::Get(Target.GetObject());
            co_await Dispatcher.DisplayMessage(FText::FormatNamed(*MessageFormat, "Pkmn", Target->GetNickname()));
        }

        co_return true;
}

UE5Coro::TCoroutine<bool> UBattleStatusEffectUtils::RemoveStatusEffectFromBattler(
    const TScriptInterface<IBattler> &Target,
    FStatusHandle StatusEffect, FForceLatentCoroutine) {
    auto &Status = Target->GetStatusEffect();
    if (!Status.IsSet()) {
        co_return false;
    }

    if (Status->StatusEffectID != StatusEffect) {
        co_return false;
    }

    if (!Target->GetAbilityComponent()->RemoveActiveGameplayEffect(Status->EffectHandle)) {
        co_return false;
    }

    auto Settings = GetDefault<UPokemonMessageSettings>();
    if (auto MessageFormat = Settings->StatusEffectCuredMessages.Find(StatusEffect); MessageFormat != nullptr) {
        auto &Dispatcher = IPokemonCoroutineDispatcher::Get(Target.GetObject());
        co_await Dispatcher.DisplayMessage(FText::FormatNamed(*MessageFormat, "Pkmn", Target->GetNickname()));
    }

    co_return true;
}