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
#include "RetroLib/Casting/DynamicCast.h"
#include "RetroLib/Optionals/PtrOrNull.h"
#include "RetroLib/Optionals/Transform.h"
#include "RetroLib/Ranges/Algorithm/To.h"
#include "RetroLib/Utils/MakeWeak.h"

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

Retro::TGenerator<TScriptInterface<IBattler>> UPokemonBattleMove::GetAllPossibleTargets() const {
    TArray<TScriptInterface<IBattler>> Targets;
    auto UserSide = Owner->GetOwningSide();
    auto UserId = Owner->GetInternalId();
    auto &Battle = UserSide->GetOwningBattle();
    // clang-format off
    co_yield Retro::Ranges::TElementsOf(Battle->GetActiveBattlers() |
           Retro::Ranges::Views::Filter([UserId](const TScriptInterface<IBattler> &Battler) {
               return Battler->GetInternalId() != UserId;
           }));
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

UE5Coro::TCoroutine<> UPokemonBattleMove::TryActivateMove(const TArray<FTargetWithIndex> &Targets) {
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
                          Retro::Ranges::Views::Transform(&FTargetWithIndex::SwapIfNecessary) |
                          Retro::Ranges::Views::Filter(Retro::ValidPtr) |
                          Retro::Ranges::Views::Transform(Retro::DynamicCastChecked<AActor>) |
                          Retro::Ranges::Views::Transform(Retro::MakeWeak) |
                          Retro::Ranges::To<TArray>());
    // clang-format on
    EventData.TargetData.Data.Emplace(TargetData);

    co_await Pokemon::Battle::Events::SendOutActivationEvent(AbilityComponent, FunctionCode,
                                                             Pokemon::Battle::Moves::UsingMove, EventData);
}