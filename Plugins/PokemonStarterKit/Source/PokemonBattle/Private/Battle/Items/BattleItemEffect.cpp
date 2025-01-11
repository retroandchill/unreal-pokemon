// "Unreal Pokémon" created by Retro & Chill.

#include "Battle/Items/BattleItemEffect.h"
#include "AbilitySystemComponent.h"
#include "Algo/ForEach.h"
#include "Bag/Item.h"
#include "Battle/Animations/BattleSequencer.h"
#include "Battle/Battlers/Battler.h"
#include "Battle/Events/UseItemPayload.h"
#include "Battle/Items/ItemTags.h"
#include "DataManager.h"
#include "Managers/PokemonSubsystem.h"
#include "Player/Bag.h"
#include "RetroLib/Casting/DynamicCast.h"
#include "RetroLib/Ranges/Algorithm/To.h"
#include "RetroLib/Utils/MakeStrong.h"

UBattleItemEffect::UBattleItemEffect() {
    auto &AbilityTrigger = AbilityTriggers.Emplace_GetRef();
    AbilityTrigger.TriggerTag = Pokemon::Battle::Items::UsingItem;
}

const FItem &UBattleItemEffect::GetItem() const {
    return FDataManager::GetInstance().GetDataTable<FItem>().GetDataChecked(ItemID);
}

bool UBattleItemEffect::ShouldAbilityRespondToEvent(const FGameplayAbilityActorInfo *ActorInfo,
                                                    const FGameplayEventData *Payload) const {
    auto ItemPayload = CastChecked<UUseItemPayload>(Payload->OptionalObject);
    return ItemPayload->Ability == this;
}

UE5Coro::GAS::FAbilityCoroutine UBattleItemEffect::ExecuteAbility(FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo *ActorInfo, FGameplayAbilityActivationInfo ActivationInfo,
    const FGameplayEventData *TriggerEventData) {
    ActorInfo->AbilitySystemComponent->AddLooseGameplayTag(Pokemon::Battle::Items::UsingItem);

    check(TriggerEventData != nullptr)
    ItemID = CastChecked<UUseItemPayload>(TriggerEventData->OptionalObject)->Item;

    TScriptInterface<IBattler> User = ActorInfo->OwnerActor.Get();
    bShouldConsumeItem = ApplyGlobalEffect(User);
    auto Targets = FilterInvalidTargets(TriggerEventData);
    Algo::ForEach(Targets, [&User, this](const TScriptInterface<IBattler> &Target) {
        bShouldConsumeItem |= ApplyEffectToTarget(User, Target);
    });

    co_await ABattleSequencer::DisplayBattleMessages();
    ActorInfo->AbilitySystemComponent->RemoveLooseGameplayTag(Pokemon::Battle::Items::UsingItem);
    if (auto &ItemData = GetItem(); bShouldConsumeItem && ItemData.Consumable) {
        auto &Subsystem = UPokemonSubsystem::GetInstance(GetCurrentActorInfo()->AvatarActor.Get());
        Subsystem.GetBag()->RemoveItem(ItemID);
    }
}

bool UBattleItemEffect::ApplyEffectToTarget_Implementation(const TScriptInterface<IBattler> &User,
                                                           const TScriptInterface<IBattler> &Target) {
    return false;
}

bool UBattleItemEffect::ApplyGlobalEffect_Implementation(const TScriptInterface<IBattler> &User) {
    return false;
}

bool UBattleItemEffect::IsTargetValid_Implementation(const TScriptInterface<IBattler> &Battler) {
    return true;
}

TArray<TScriptInterface<IBattler>> UBattleItemEffect::FilterInvalidTargets(const FGameplayEventData *TriggerEventData) {
    // clang-format on
    return TriggerEventData->TargetData.Data | Retro::Ranges::Views::Transform(&FGameplayAbilityTargetData::GetActors) |
           Retro::Ranges::Views::Join | Retro::Ranges::Views::Transform(Retro::MakeStrongChecked) |
           Retro::Ranges::Views::Filter(Retro::ValidPtr) | Retro::Ranges::Views::Filter(Retro::InstanceOf<IBattler>) |
           Retro::Ranges::Views::Transform(Retro::DynamicCastChecked<IBattler>) |
           Retro::Ranges::Views::Transform(Retro::WrapPointer) |
           Retro::Ranges::Views::Filter(Retro::BindMethod<&UBattleItemEffect::IsTargetValid>(this)) |
           Retro::Ranges::To<TArray>();
    // clang-format off
}