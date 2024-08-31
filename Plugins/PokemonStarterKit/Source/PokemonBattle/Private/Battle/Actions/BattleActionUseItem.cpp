// "Unreal Pokémon" created by Retro & Chill.

#include "Battle/Actions/BattleActionUseItem.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Bag/Item.h"
#include "Battle/Battlers/Battler.h"
#include "Battle/Battlers/BattlerAbilityComponent.h"
#include "Battle/Events/UseItemPayload.h"
#include "Battle/Items/ItemLookup.h"
#include "Battle/Items/ItemTags.h"
#include "DataManager.h"
#include "PokemonBattleSettings.h"
#include "Ranges/Algorithm/ToArray.h"
#include "Ranges/Optional/OrElseGet.h"
#include "Ranges/Views/CastType.h"
#include "Ranges/Views/ContainerView.h"
#include "Ranges/Views/FilterValid.h"
#include "Ranges/Views/MakeWeak.h"

FItemTarget::FItemTarget(TWeakInterfacePtr<IBattler> &&Battler) {
    Data.Set<TWeakInterfacePtr<IBattler>>(MoveTemp(Battler));
}

FItemTarget::FItemTarget(TWeakInterfacePtr<IBattleMove> &&Move) {
    Data.Set<TWeakInterfacePtr<IBattleMove>>(MoveTemp(Move));
}

FItemTarget::FItemTarget(FTargetWithIndex &&Target) {
    Data.Set<FTargetWithIndex>(MoveTemp(Target));
}

FBattleActionUseItem::FBattleActionUseItem(const TScriptInterface<IBattler> &Battler, FName ItemID,
                                           FItemTarget &&ItemTarget)
    : FBattleActionBase(Battler), ItemID(ItemID), ItemTarget(MoveTemp(ItemTarget)) {
}

FString FBattleActionUseItem::GetReferencerName() const {
    return TEXT("FBattleActionUseItem");
}

int32 FBattleActionUseItem::GetPriority() const {
    return GetDefault<UPokemonBattleSettings>()->SwitchPriority;
}

FText FBattleActionUseItem::GetActionMessage() const {
    auto &ItemData = FDataManager::GetInstance().GetDataTable<FItem>().GetDataChecked(ItemID);
    return FText::Format(FText::FromStringView(TEXT("{0} used {1}!")),
                         {GetBattler()->GetWrappedPokemon()->GetCurrentHandler()->GetTrainerName(), ItemData.RealName});
}

FGameplayAbilitySpecHandle FBattleActionUseItem::ActivateAbility() {
    auto EffectClass = Pokemon::Battle::Items::FindBattleItemEffect(ItemID);
    auto &Owner = GetBattler();
    auto AbilityComponent = Owner->GetAbilityComponent();
    auto ExistingHandle = AbilityComponent->FindAbilityOfClass(EffectClass);
    auto Handle = ExistingHandle | UE::Optionals::OrElseGet([&EffectClass, &Owner, &AbilityComponent] {
                      FGameplayAbilitySpec Spec(EffectClass, 1, INDEX_NONE, Owner.GetObject());
                      return AbilityComponent->GiveAbility(Spec);
                  });

    auto OwnerActor = CastChecked<AActor>(Owner.GetObject());
    FGameplayEventData EventData;
    EventData.Instigator = OwnerActor;

    auto Payload = NewObject<UUseItemPayload>();
    Payload->Item = ItemID;
    bool bIsInstanced;
    Payload->Ability =
        UAbilitySystemBlueprintLibrary::GetGameplayAbilityFromSpecHandle(AbilityComponent, Handle, bIsInstanced);
    EventData.OptionalObject = Payload;
    auto TargetData = MakeShared<FGameplayAbilityTargetData_ActorArray>();
    TArray<TScriptInterface<IBattler>> Targets;
    if (auto AsBattler = ItemTarget.Data.TryGet<TWeakInterfacePtr<IBattler>>(); AsBattler != nullptr) {
        Targets.Emplace(AsBattler->ToScriptInterface());
    } else if (auto AsMove = ItemTarget.Data.TryGet<TWeakInterfacePtr<IBattleMove>>(); AsMove != nullptr) {
        auto &MoveOwner = AsMove->Get()->GetOwningBattler();
        Targets.Emplace(MoveOwner);
        Payload->TargetedMoves.Add(MoveOwner->GetInternalId(), AsMove->ToScriptInterface());
    } else if (auto AsTargetWithIndex = ItemTarget.Data.TryGet<FTargetWithIndex>(); AsTargetWithIndex != nullptr) {
        Targets.Emplace(AsTargetWithIndex->SwapIfNecessary());
    }

    TargetData->SetActors(Targets | UE::Ranges::FilterValid | UE::Ranges::CastType<AActor> | UE::Ranges::MakeWeak |
                          UE::Ranges::ToArray);
    EventData.TargetData.Data.Emplace(TargetData);

    UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(OwnerActor, Pokemon::Battle::Items::UsingItem, EventData);

    return Handle;
}