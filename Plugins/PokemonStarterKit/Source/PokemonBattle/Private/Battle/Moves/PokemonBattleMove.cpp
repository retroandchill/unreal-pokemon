// "Unreal Pokémon" created by Retro & Chill.

#include "Battle/Moves/PokemonBattleMove.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Battle/Battle.h"
#include "Battle/Battlers/Battler.h"
#include "Battle/Battlers/BattlerAbilityComponent.h"
#include "Battle/BattleSide.h"
#include "Battle/Events/Moves/UseMovePayload.h"
#include "Battle/Moves/BattleMoveFunctionCode.h"
#include "Battle/Moves/MoveLookup.h"
#include "Battle/Moves/MoveTags.h"
#include "Moves/MoveData.h"
#include "Pokemon/Moves/Move.h"
#include "Ranges/Algorithm/ToArray.h"
#include "Ranges/Utilities/Casts.h"
#include "Ranges/Views/CastType.h"
#include "Ranges/Views/ContainerView.h"
#include "Ranges/Views/Filter.h"
#include "Ranges/Views/FilterValid.h"
#include "Ranges/Views/MakeWeak.h"

TScriptInterface<IBattleMove> UPokemonBattleMove::Initialize(const TScriptInterface<IBattler> &Battler,
                                                             const TScriptInterface<IMove> &Move) {
    Owner = Battler;
    WrappedMove = Move;

    auto AbilityComponent = Owner->GetAbilityComponent();
    auto FunctionCodeClass = Pokemon::Battle::Moves::LookupMoveEffectClass(WrappedMove->GetFunctionCode());
    if (auto AbilityObject = AbilityComponent->FindAbilitySpecFromClass(FunctionCodeClass); AbilityObject != nullptr) {
        FunctionCode = AbilityObject->Handle;
    } else {
        FGameplayAbilitySpec Spec(FunctionCodeClass, 1, INDEX_NONE, this);
        FunctionCode = AbilityComponent->GiveAbility(Spec);
    }

    return this;
}

bool UPokemonBattleMove::IsUsable() const {
    return WrappedMove->GetCurrentPP() > 0;
}

UE::Ranges::TAnyView<TScriptInterface<IBattler>> UPokemonBattleMove::GetAllPossibleTargets() const {
    TArray<TScriptInterface<IBattler>> Targets;
    auto UserSide = Owner->GetOwningSide();
    auto UserId = Owner->GetInternalId();
    auto &Battle = UserSide->GetOwningBattle();
    // clang-format off
    return Battle->GetActiveBattlers() |
           UE::Ranges::Filter([UserId](const TScriptInterface<IBattler> &Battler) {
               return Battler->GetInternalId() != UserId;
           });
    // clang-format on
}

FText UPokemonBattleMove::GetDisplayName() const {
    return WrappedMove->GetDisplayName();
}

int32 UPokemonBattleMove::GetCurrentPP() const {
    return WrappedMove->GetCurrentPP();
}

int32 UPokemonBattleMove::GetMaxPP() const {
    return WrappedMove->GetTotalPP();
}

FName UPokemonBattleMove::GetDisplayType() const {
    return WrappedMove->GetType();
}

int32 UPokemonBattleMove::GetBasePower() const {
    return WrappedMove->GetBasePower();
}

int32 UPokemonBattleMove::GetAccuracy() const {
    return WrappedMove->GetAccuracy();
}

EMoveDamageCategory UPokemonBattleMove::GetCategory() const {
    return WrappedMove->GetDamageCategory();
}

const FMoveTarget &UPokemonBattleMove::GetTargetType() const {
    return WrappedMove->GetTargetType();
}

const TArray<FName> &UPokemonBattleMove::GetTags() const {
    return WrappedMove->GetTags();
}

int32 UPokemonBattleMove::GetPriority() const {
    return WrappedMove->GetMoveData().Priority;
}

int32 UPokemonBattleMove::GetAdditionalEffectChance() const {
    return WrappedMove->GetAdditionalEffectChance();
}

void UPokemonBattleMove::PayCost(int32 Amount) {
    WrappedMove->DecrementPP(Amount);
}

const TScriptInterface<IBattler> &UPokemonBattleMove::GetOwningBattler() const {
    return Owner;
}

FGameplayAbilitySpecHandle UPokemonBattleMove::TryActivateMove(const TArray<FTargetWithIndex> &Targets) {
    auto AbilityComponent = Owner->GetAbilityComponent();
    auto OwnerActor = CastChecked<AActor>(Owner.GetObject());
    FGameplayEventData EventData;
    EventData.Instigator = OwnerActor;

    auto Payload = NewObject<UUseMovePayload>();
    Payload->Move = this;
    bool bIsInstanced;
    Payload->Ability =
        UAbilitySystemBlueprintLibrary::GetGameplayAbilityFromSpecHandle(AbilityComponent, FunctionCode, bIsInstanced);
    EventData.OptionalObject = Payload;
    auto TargetData = MakeShared<FGameplayAbilityTargetData_ActorArray>();
    // clang-format off
    TargetData->SetActors(Targets |
                          UE::Ranges::Map(&FTargetWithIndex::SwapIfNecessary) |
                          UE::Ranges::Filter(UE::Ranges::ValidPtr) |
                          UE::Ranges::Map(UE::Ranges::DynamicCastChecked<AActor>) |
                          UE::Ranges::MakeWeak |
                          UE::Ranges::ToArray);
    // clang-format on
    EventData.TargetData.Data.Emplace(TargetData);

    UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(OwnerActor, Pokemon::Battle::Moves::UsingMove, EventData);
    return FunctionCode;
}