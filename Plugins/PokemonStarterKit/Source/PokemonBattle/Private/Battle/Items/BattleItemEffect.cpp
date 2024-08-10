// "Unreal Pokémon" created by Retro & Chill.


#include "Battle/Items/BattleItemEffect.h"
#include "DataManager.h"
#include "RangeHelpers.h"
#include "Battle/Items/ItemTags.h"
#include "Bag/Item.h"
#include "Battle/Battlers/Battler.h"
#include "Battle/Events/UseItemPayload.h"
#include "Kismet/GameplayStatics.h"
#include "Managers/PokemonSubsystem.h"
#include "Player/Bag.h"
#include <range/v3/view/filter.hpp>
#include <range/v3/view/join.hpp>
#include <range/v3/view/transform.hpp>

UBattleItemEffect::UBattleItemEffect() {
    auto &AbilityTrigger = AbilityTriggers.Emplace_GetRef();
    AbilityTrigger.TriggerTag = Pokemon::Battle::Items::UsingItem;
}

const FItem & UBattleItemEffect::GetItem() const {
    return FDataManager::GetInstance().GetDataTable<FItem>().GetDataChecked(ItemID);
}

bool UBattleItemEffect::ShouldAbilityRespondToEvent(const FGameplayAbilityActorInfo *ActorInfo,
    const FGameplayEventData *Payload) const {
    auto ItemPayload = CastChecked<UUseItemPayload>(Payload->OptionalObject);
    return ItemPayload->Ability == this;
}

void UBattleItemEffect::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo *ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
    const FGameplayEventData *TriggerEventData) {
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    check(TriggerEventData != nullptr)
    ItemID = CastChecked<UUseItemPayload>(TriggerEventData->OptionalObject)->Item;

    auto Targets = FilterInvalidTargets(TriggerEventData);
    ApplyAbilityEffect(ActorInfo->OwnerActor.Get(), Targets);
}

void UBattleItemEffect::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo *ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) {
    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UBattleItemEffect::FinishAbility(bool bShouldConsumeItem) {
    if (auto &ItemData = GetItem(); bShouldConsumeItem && ItemData.Consumable) {
        auto &Subsystem = UPokemonSubsystem::GetInstance(GetCurrentActorInfo()->AvatarActor.Get());
        Subsystem.GetBag()->RemoveItem(ItemID);
    }
    EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), false, false);
}

bool UBattleItemEffect::IsTargetValid_Implementation(const TScriptInterface<IBattler> &Battler) {
    return true;
}

TArray<TScriptInterface<IBattler>> UBattleItemEffect::FilterInvalidTargets(
    const FGameplayEventData* TriggerEventData) {
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
           ranges::views::filter(&AActor::Implements<UBattler>) |
               ranges::views::transform([](AActor *Actor) {
               TScriptInterface<IBattler> Battler = Actor;
               return Battler;
           }) | ranges::views::filter(std::bind_front(&UBattleItemEffect::IsTargetValid, this)) |
           RangeHelpers::TToArray<TScriptInterface<IBattler>>();
}