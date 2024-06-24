// "Unreal Pokémon" created by Retro & Chill.


#include "Battle/GameplayAbilities/Component/SameTypeAttackBonusComponent.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemLog.h"
#include "DataManager.h"
#include "RangeHelpers.h"
#include "Algo/AnyOf.h"
#include "Battle/Type.h"
#include "Battle/Battlers/Battler.h"
#include <range/v3/view/concat.hpp>
#include <range/v3/view/filter.hpp>
#include <range/v3/view/join.hpp>
#include <range/v3/view/single.hpp>
#include <range/v3/view/transform.hpp>

bool IsStabApplicable(const UAbilitySystemComponent* Owner) {
    FGameplayTagContainer OwnedTags;
    Owner->GetOwnedGameplayTags(OwnedTags);

    // See if we should be inhibiting the execution
    TScriptInterface<IBattler> Battler = Owner->GetOwner();
    check(Battler != nullptr)

    using namespace ranges::views;
    static const auto &DataTable = FDataManager::GetInstance().GetDataTable<FType>();
    auto TagPairs = RangeHelpers::CreateRange(Battler->GetTypes())
        | transform([](FName Type) { return DataTable.GetData(Type); })
        | filter([](const FType* Type) { return Type->UseMoveTag.IsValid() && Type->BattlerTag.IsValid(); })
        | transform([](const FType* Type) {
            FGameplayTagContainer Container;
            Container.AddTag(Type->UseMoveTag);
            Container.AddTag(Type->BattlerTag);
            return Container;
        })
        | RangeHelpers::TToArray<FGameplayTagContainer>();
    
    return TagPairs.IsEmpty() ||
        Algo::AnyOf(TagPairs, [&OwnedTags](const FGameplayTagContainer& Tags) { return OwnedTags.HasAll(Tags); });
}

bool USameTypeAttackBonusComponent::OnActiveGameplayEffectAdded(FActiveGameplayEffectsContainer &ActiveGEContainer,
    FActiveGameplayEffect &ActiveGE) const {
    auto ASC = ActiveGEContainer.Owner;
	check(ASC != nullptr)

	auto ActiveGEHandle = ActiveGE.Handle;
	if (auto EventSet = ASC->GetActiveEffectEventSet(ActiveGEHandle); EventSet != nullptr) {
	    using namespace ranges::views;
	    static const auto &DataTable = FDataManager::GetInstance().GetDataTable<FType>();
	    auto AllRows = DataTable.GetAllRows();
	    auto GameplayTagsToBind = RangeHelpers::CreateRange(AllRows)
	        | transform([](const FType* Type) { return concat(single(Type->UseMoveTag), single(Type->BattlerTag)); })
	        | join
            | filter([](const FGameplayTag& Tag) { return Tag.IsValid(); })
            | RangeHelpers::TToArray<FGameplayTag>();

		// Add our tag requirements to the ASC's Callbacks map. This helps filter down the amount of callbacks we'll get due to tag changes
		// (rather than registering for the one callback whenever any tag changes).  We also need to keep track to remove those registered delegates in OnEffectRemoved.
		TArray<TTuple<FGameplayTag, FDelegateHandle>> AllBoundEvents;
		for (const FGameplayTag& Tag : GameplayTagsToBind) {
			FOnGameplayEffectTagCountChanged& OnTagEvent = ASC->RegisterGameplayTagEvent(Tag, EGameplayTagEventType::NewOrRemoved);
			FDelegateHandle Handle = OnTagEvent.AddUObject(this, &USameTypeAttackBonusComponent::OnTagChanged, ActiveGEHandle);
			AllBoundEvents.Emplace(Tag, Handle);
		}

		// Now when this Effect is removed, we should remove all of our registered callbacks.
		EventSet->OnEffectRemoved.AddUObject(this, &USameTypeAttackBonusComponent::OnGameplayEffectRemoved, ASC, MoveTemp(AllBoundEvents));
	} else {
		UE_LOG(LogGameplayEffects, Error, TEXT("USameTypeAttackBonusComponent::OnGameplayEffectAdded called with ActiveGE: %s which had an invalid FActiveGameplayEffectHandle."), *ActiveGE.GetDebugString());
	}

	FGameplayTagContainer TagContainer;
	ASC->GetOwnedGameplayTags(TagContainer);
	return IsStabApplicable(ASC);
}

void USameTypeAttackBonusComponent::OnGameplayEffectRemoved(const FGameplayEffectRemovalInfo &GERemovalInfo,
                                                            UAbilitySystemComponent *ASC, TArray<TTuple<FGameplayTag, FDelegateHandle>> AllBoundEvents) const {
    for (auto& [Tag, Delegate] : AllBoundEvents){
        const bool bSuccess = ASC->UnregisterGameplayTagEvent(Delegate, Tag, EGameplayTagEventType::NewOrRemoved);
        UE_CLOG(!bSuccess, LogGameplayEffects, Error, TEXT("%s tried to unregister GameplayTagEvent '%s' on GameplayEffect '%s' but failed."),
            *GetName(), *Tag.ToString(), *GetNameSafe(GetOwner()));
    }
}

void USameTypeAttackBonusComponent::OnTagChanged(const FGameplayTag GameplayTag, int32 NewCount,
    FActiveGameplayEffectHandle ActiveGEHandle) const {
    // Note: This function can remove us (RemoveActiveGameplayEffect eventually calling OnGameplayEffectRemoved),
    // but we could be in the middle of a stack of OnTagChanged callbacks, wo we could get a stale OnTagChanged.
    auto Owner = ActiveGEHandle.GetOwningAbilitySystemComponent();
    if (!Owner) {
        return;
    }

    // It's possible for this to return nullptr if it was in the process of being removed (IsPendingRemove)
    if (const auto ActiveGE = Owner->GetActiveGameplayEffect(ActiveGEHandle); !ActiveGE) {
        return;
    }
    
    constexpr bool bInvokeCuesIfStateChanged = true;
    const bool bOngoingRequirementsMet = IsStabApplicable(Owner);
    Owner->SetActiveGameplayEffectInhibit(MoveTemp(ActiveGEHandle), !bOngoingRequirementsMet, bInvokeCuesIfStateChanged);
}