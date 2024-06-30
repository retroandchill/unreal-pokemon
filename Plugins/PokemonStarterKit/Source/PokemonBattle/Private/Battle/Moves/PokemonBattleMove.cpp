// "Unreal Pokémon" created by Retro & Chill.

#include "Battle/Moves/PokemonBattleMove.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Battle/Battle.h"
#include "Battle/Battlers/Battler.h"
#include "Battle/BattleSide.h"
#include "Moves/MoveData.h"
#include "Pokemon/Moves/Move.h"
#include "RangeHelpers.h"
#include "Battle/GameplayAbilities/BattlerAbilityComponent.h"
#include <range/v3/view/filter.hpp>
#include <range/v3/view/transform.hpp>
#include "Battle/Moves/BattleMoveFunctionCode.h"
#include "Battle/Moves/MoveLookup.h"
#include "Battle/Moves/MoveTags.h"
#include "Battle/Moves/UseMovePayload.h"

TScriptInterface<IBattleMove> UPokemonBattleMove::Initialize(const TScriptInterface<IBattler>& Battler,
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

TArray<TScriptInterface<IBattler>> UPokemonBattleMove::GetAllPossibleTargets() const {
    TArray<TScriptInterface<IBattler>> Targets;
    auto UserSide = Owner->GetOwningSide();
    auto UserId = Owner->GetInternalId();
    auto &Battle = UserSide->GetOwningBattle();
    return Battle->GetActiveBattlers() |
           ranges::views::filter(
               [&UserId](const TScriptInterface<IBattler> &Battler) { return Battler->GetInternalId() != UserId; }) |
           RangeHelpers::TToArray<TScriptInterface<IBattler>>();
}

FText UPokemonBattleMove::GetDisplayName() const {
    return WrappedMove->GetMoveData().RealName;
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

const FMoveTarget & UPokemonBattleMove::GetTargetType() const {
    return WrappedMove->GetTargetType();
}

const TArray<FName> & UPokemonBattleMove::GetTags() const {
    return WrappedMove->GetTags();
}

int32 UPokemonBattleMove::GetPriority() const {
    return WrappedMove->GetMoveData().Priority;
}

void UPokemonBattleMove::PayCost(int32 Amount) {
    WrappedMove->DecrementPP(Amount);
}

const TScriptInterface<IBattler> &UPokemonBattleMove::GetOwningBattler() const {
    return Owner;
}

FGameplayAbilitySpecHandle UPokemonBattleMove::TryActivateMove(const TArray<TScriptInterface<IBattler>> &Targets) {
    auto AbilityComponent = Owner->GetAbilityComponent();
    auto OwnerActor = CastChecked<AActor>(Owner.GetObject());
    FGameplayEventData EventData;
    EventData.Instigator = OwnerActor;

    auto Payload = NewObject<UUseMovePayload>();
    Payload->Move = this;
    bool bIsInstanced;
    Payload->Ability = UAbilitySystemBlueprintLibrary::GetGameplayAbilityFromSpecHandle(AbilityComponent, FunctionCode, bIsInstanced);
    EventData.OptionalObject = Payload;
    auto TargetData = MakeShared<FGameplayAbilityTargetData_ActorArray>();
    TargetData->SetActors(RangeHelpers::CreateRange(Targets)
        | ranges::views::transform([](const TScriptInterface<IBattler>& Battler) { return CastChecked<AActor>(Battler.GetObject()); })
        | RangeHelpers::TToArray<TWeakObjectPtr<AActor>>());
    EventData.TargetData.Data.Emplace(TargetData);
    
    UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(OwnerActor, Pokemon::Battle::Moves::UsingMove, EventData);
    return FunctionCode;
}