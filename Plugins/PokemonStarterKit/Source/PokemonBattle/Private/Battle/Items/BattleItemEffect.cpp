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
#include "Battle/Animations/BattleSequencer.h"
#include "Kismet/GameplayStatics.h"
#include "Managers/PokemonSubsystem.h"
#include "Player/Bag.h"
#include "Ranges/Algorithm/ToArray.h"
#include "Ranges/Views/CacheLast.h"
#include "Ranges/Views/CastType.h"
#include "Ranges/Views/ContainerView.h"
#include "Ranges/Views/Filter.h"
#include "Ranges/Views/FilterValid.h"
#include "Ranges/Views/Join.h"
#include "Ranges/Views/MakeStrong.h"
#include "Ranges/Views/Map.h"

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
    bShouldConsumeItem = ApplyGlobalEffect(User);
    auto Targets = FilterInvalidTargets(TriggerEventData);
    Algo::ForEach(Targets, [&User, this](const TScriptInterface<IBattler> &Target) {
        bShouldConsumeItem |= ApplyEffectToTarget(User, Target);
    });

    ABattleSequencer::DisplayBattleMessages(this, [this] {
        ensure(CurrentActorInfo != nullptr);
        EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
    });
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
    return TriggerEventData->TargetData.Data | UE::Ranges::Map(&FGameplayAbilityTargetData::GetActors) |
           UE::Ranges::CacheLast | UE::Ranges::Join | UE::Ranges::MakeStrong | UE::Ranges::FilterValid |
           UE::Ranges::Filter(&AActor::Implements<UBattler>) | UE::Ranges::CastType<IBattler> |
           UE::Ranges::Filter(this, &UBattleItemEffect::IsTargetValid) | UE::Ranges::ToArray;
}