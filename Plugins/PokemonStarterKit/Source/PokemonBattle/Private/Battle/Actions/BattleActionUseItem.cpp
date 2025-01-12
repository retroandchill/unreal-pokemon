// "Unreal Pokémon" created by Retro & Chill.

#include "Battle/Actions/BattleActionUseItem.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Bag/Item.h"
#include "Battle/Battlers/Battler.h"
#include "Battle/Battlers/BattlerAbilityComponent.h"
#include "Battle/Events/UseItemPayload.h"
#include "Battle/Items/BattleItemEffect.h"
#include "Battle/Items/ItemLookup.h"
#include "Battle/Items/ItemTags.h"
#include "DataManager.h"
#include "Pokemon/Pokemon.h"
#include "PokemonBattleSettings.h"
#include "Battle/Events/TargetedEvents.h"
#include "RetroLib/Casting/DynamicCast.h"
#include "RetroLib/Optionals/PtrOrNull.h"
#include "RetroLib/Optionals/Transform.h"
#include "RetroLib/Ranges/Algorithm/To.h"
#include "RetroLib/Utils/MakeWeak.h"

FItemTarget::FItemTarget(TWeakInterfacePtr<IBattler> &&Battler) {
    Data.Set<TWeakInterfacePtr<IBattler>>(std::move(Battler));
}

FItemTarget::FItemTarget(TWeakInterfacePtr<IBattleMove> &&Move) {
    Data.Set<TWeakInterfacePtr<IBattleMove>>(std::move(Move));
}

FItemTarget::FItemTarget(FTargetWithIndex &&Target) {
    Data.Set<FTargetWithIndex>(std::move(Target));
}

FBattleActionUseItem::FBattleActionUseItem(const TScriptInterface<IBattler> &Battler, FName ItemID,
                                           FItemTarget &&ItemTarget)
    : FBattleActionBase(Battler), ItemID(ItemID), ItemTarget(std::move(ItemTarget)) {
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

UE5Coro::TCoroutine<> FBattleActionUseItem::ActivateAbility() {
    auto EffectClass = Pokemon::Battle::Items::FindBattleItemEffect(ItemID);
    auto &Owner = GetBattler();
    auto AbilityComponent = Owner->GetAbilityComponent();
    auto ExistingHandle = AbilityComponent->FindAbilityOfClass(EffectClass);
    // clang-format off
    auto Handle = ExistingHandle |
                  Retro::Optionals::OrElseGet([&EffectClass, &Owner, &AbilityComponent] {
                      FGameplayAbilitySpec Spec(EffectClass, 1, INDEX_NONE, Owner.GetObject());
                      return AbilityComponent->GiveAbility(Spec);
                  });
    // clang-format on

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

    // clang-format off
    TargetData->SetActors(Targets |
        Retro::Ranges::Views::Filter(Retro::ValidPtr) |
        Retro::Ranges::Views::Transform(Retro::DynamicCastChecked<AActor>) |
        Retro::Ranges::Views::Transform(Retro::MakeWeak) |
        Retro::Ranges::To<TArray>());
    // clang-format on
    EventData.TargetData.Data.Emplace(TargetData);

    co_await Pokemon::Battle::Events::SendOutActivationEvent(AbilityComponent, Handle, Pokemon::Battle::Items::UsingItem, std::move(EventData));
}