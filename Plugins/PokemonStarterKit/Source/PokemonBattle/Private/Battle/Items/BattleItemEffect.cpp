// "Unreal Pokémon" created by Retro & Chill.

#include "Battle/Items/BattleItemEffect.h"
#include "AbilitySystemComponent.h"
#include "Algo/ForEach.h"
#include "Bag/Item.h"
#include "Battle/Battlers/Battler.h"
#include "Battle/Events/BattleMessage.h"
#include "Battle/Events/UseItemPayload.h"
#include "Battle/Items/ItemTags.h"
#include "DataManager.h"
#include "Kismet/GameplayStatics.h"
#include "Managers/PokemonSubsystem.h"
#include "Player/Bag.h"
#include "RangeHelpers.h"
#include <range/v3/algorithm/for_each.hpp>
#include <range/v3/view/filter.hpp>
#include <range/v3/view/join.hpp>
#include <range/v3/view/transform.hpp>

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

void UBattleItemEffect::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                        const FGameplayAbilityActorInfo *ActorInfo,
                                        const FGameplayAbilityActivationInfo ActivationInfo,
                                        const FGameplayEventData *TriggerEventData) {
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
    ActorInfo->AbilitySystemComponent->AddLooseGameplayTag(Pokemon::Battle::Items::UsingItem);

    check(TriggerEventData != nullptr)
    ItemID = CastChecked<UUseItemPayload>(TriggerEventData->OptionalObject)->Item;

    TScriptInterface<IBattler> User = ActorInfo->OwnerActor.Get();
    FRunningMessageSet Messages;
    bShouldConsumeItem = ApplyGlobalEffect(User, Messages);
    auto Targets = FilterInvalidTargets(TriggerEventData);
    Algo::ForEach(Targets, [&User, &Messages, this](const TScriptInterface<IBattler> &Target) {
        bShouldConsumeItem |= ApplyEffectToTarget(User, Target, Messages);
    });
    DisplayResults(Messages);
}

void UBattleItemEffect::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo *ActorInfo,
                                   const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility,
                                   bool bWasCancelled) {
    ActorInfo->AbilitySystemComponent->RemoveLooseGameplayTag(Pokemon::Battle::Items::UsingItem);
    if (auto &ItemData = GetItem(); bShouldConsumeItem && ItemData.Consumable) {
        auto &Subsystem = UPokemonSubsystem::GetInstance(GetCurrentActorInfo()->AvatarActor.Get());
        Subsystem.GetBag()->RemoveItem(ItemID);
    }
    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

bool UBattleItemEffect::ApplyEffectToTarget_Implementation(const TScriptInterface<IBattler> &User,
                                                           const TScriptInterface<IBattler> &Target,
                                                           const FRunningMessageSet &Messages) {
    return false;
}

bool UBattleItemEffect::ApplyGlobalEffect_Implementation(const TScriptInterface<IBattler> &User,
                                                         const FRunningMessageSet &Messages) {
    return false;
}

bool UBattleItemEffect::IsTargetValid_Implementation(const TScriptInterface<IBattler> &Battler) {
    return true;
}

TArray<TScriptInterface<IBattler>> UBattleItemEffect::FilterInvalidTargets(const FGameplayEventData *TriggerEventData) {
    auto ActorLists =
        RangeHelpers::CreateRange(TriggerEventData->TargetData.Data) |
        ranges::views::transform([](const TSharedPtr<FGameplayAbilityTargetData> &Ptr) { return Ptr->GetActors(); }) |
        RangeHelpers::TToArray<TArray<TWeakObjectPtr<AActor>>>();

    return RangeHelpers::CreateRange(ActorLists) |
           ranges::views::transform(
               [](const TArray<TWeakObjectPtr<AActor>> &List) { return RangeHelpers::CreateRange(List); }) |
           ranges::views::join |
           ranges::views::transform([](const TWeakObjectPtr<AActor> &Actor) { return Actor.Get(); }) |
           ranges::views::filter([](const AActor *Actor) { return Actor != nullptr; }) |
           ranges::views::filter(&AActor::Implements<UBattler>) | ranges::views::transform([](AActor *Actor) {
               TScriptInterface<IBattler> Battler = Actor;
               return Battler;
           }) |
           ranges::views::filter(std::bind_front(&UBattleItemEffect::IsTargetValid, this)) |
           RangeHelpers::TToArray<TScriptInterface<IBattler>>();
}