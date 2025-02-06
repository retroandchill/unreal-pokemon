// "Unreal Pokémon" created by Retro & Chill.

#include "Battle/Stats/RefreshStatsOnTagChangeGameplayEffectComponent.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"
#include "PokemonBattleModule.h"
#include "Algo/Find.h"

bool URefreshStatsOnTagChangeGameplayEffectComponent::OnActiveGameplayEffectAdded(
    FActiveGameplayEffectsContainer &ActiveGEContainer, FActiveGameplayEffect &ActiveGE) const {
    auto ASC = ActiveGEContainer.Owner;
    check(ASC != nullptr)

    FActiveGameplayEffectHandle ActiveGEHandle = ActiveGE.Handle;
    auto EventSet = ASC->GetActiveEffectEventSet(ActiveGEHandle);
    check(EventSet != nullptr)

    // Quick method of appending a TArray to another TArray with no duplicates.
    auto AppendUnique = [](TArray<FGameplayTag> &Destination, const TArray<FGameplayTag> &Source) {
        // Make sure the array won't allocate during the loop
        if (Destination.GetSlack() < Source.Num()) {
            Destination.Reserve(Destination.Num() + Source.Num());
        }
        const TConstArrayView<FGameplayTag> PreModifiedDestinationView{Destination.GetData(), Destination.Num()};

        for (const FGameplayTag &Tag : Source) {
            if (!Algo::Find(PreModifiedDestinationView, Tag)) {
                Destination.Emplace(Tag);
            }
        }
    };

    // We should gather a list of tags to listen on events for
    TArray<FGameplayTag> GameplayTagsToBind;
    for (const auto &[AffectedAttribute, AffectingTags] : StatsToUpdate) {
        AppendUnique(GameplayTagsToBind, AffectingTags.GetGameplayTagArray());
    }

    TArray<TTuple<FGameplayTag, FDelegateHandle>> AllBoundEvents;
    for (const FGameplayTag &Tag : GameplayTagsToBind) {
        FOnGameplayEffectTagCountChanged &OnTagEvent =
            ASC->RegisterGameplayTagEvent(Tag, EGameplayTagEventType::NewOrRemoved);
        FDelegateHandle Handle =
            OnTagEvent.AddUObject(this, &URefreshStatsOnTagChangeGameplayEffectComponent::OnTagChanged, ActiveGEHandle);
        AllBoundEvents.Emplace(Tag, Handle);
    }

    // Now when this Effect is removed, we should remove all of our registered callbacks.
    EventSet->OnEffectRemoved.AddUObject(this,
                                         &URefreshStatsOnTagChangeGameplayEffectComponent::OnGameplayEffectRemoved, ASC,
                                         std::move(AllBoundEvents));

    return true;
}

void URefreshStatsOnTagChangeGameplayEffectComponent::OnTagChanged(const FGameplayTag GameplayTag, int32 NewCount,
                                                                   FActiveGameplayEffectHandle ActiveGEHandle) const {
    auto OwningComponent = ActiveGEHandle.GetOwningAbilitySystemComponent();
    for (const auto &[AffectedAttribute, AffectingTags] : StatsToUpdate) {
        if (!AffectingTags.HasTag(GameplayTag)) {
            continue;
        }

        // Set the base to the same value for doing this should theoretically force a refresh
        float CurrentBase = OwningComponent->GetNumericAttributeBase(AffectedAttribute);
        OwningComponent->SetNumericAttributeBase(AffectedAttribute, CurrentBase);
    }
}

void URefreshStatsOnTagChangeGameplayEffectComponent::OnGameplayEffectRemoved(
    const FGameplayEffectRemovalInfo &GERemovalInfo, UAbilitySystemComponent *ASC,
    TArray<TTuple<FGameplayTag, FDelegateHandle>> AllBoundEvents) const {
    for (const auto &[Key, Value] : AllBoundEvents) {
        const bool bSuccess = ASC->UnregisterGameplayTagEvent(Value, Key, EGameplayTagEventType::NewOrRemoved);
        UE_CLOG(!bSuccess, LogBattle, Error,
                TEXT("%s tried to unregister GameplayTagEvent '%s' on GameplayEffect '%s' but failed."), *GetName(),
                *Key.ToString(), *GetNameSafe(GetOwner()));
    }
}