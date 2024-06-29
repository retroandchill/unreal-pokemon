// "Unreal Pokémon" created by Retro & Chill.

#include "Battle/PokemonBattle.h"
#include "Algo/AnyOf.h"
#include "Algo/ForEach.h"
#include "Algo/NoneOf.h"
#include "Battle/Actions/BattleAction.h"
#include "Battle/Battlers/Battler.h"
#include "Battle/BattleSide.h"
#include "Lookup/InjectionUtilities.h"
#include "Managers/PokemonSubsystem.h"
#include "Pokemon/Pokemon.h"
#include "range/v3/view/join.hpp"
#include "range/v3/view/transform.hpp"
#include "RangeHelpers.h"
#include <functional>
#include <range/v3/view/filter.hpp>
#include <range/v3/view/empty.hpp>

static auto GetBattlers(const TScriptInterface<IBattleSide> &Side) {
    return RangeHelpers::CreateRange(Side->GetBattlers());
}

static bool IsNotFainted(const TScriptInterface<IBattler> &Battler) {
    return !Battler->IsFainted();
}

void APokemonBattle::CreateWildBattle(const FPokemonDTO &Pokemon) {
    TScriptInterface<IBattle> Self = this;
    auto PlayerSide = GetWorld()->SpawnActor<AActor>(BattleSideClass.LoadSynchronous(), GetPlayerSidePosition());
    Sides.Emplace_GetRef(PlayerSide)
        ->Initialize(Self, UPokemonSubsystem::GetInstance(this).GetPlayer(), BattleSettings.BattlefieldSize.PlayerSide,
                     true);
    PlayerSide->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepWorld, true));

    auto OpponentSide = GetWorld()->SpawnActor<AActor>(BattleSideClass.LoadSynchronous(), GetOpponentSidePosition());
    Sides.Emplace_GetRef(OpponentSide)
        ->Initialize(Self, UnrealInjector::NewInjectedDependency<IPokemon>(this, Pokemon));
    OpponentSide->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepWorld, true));
    Execute_JumpToBattleScene(this, UGameplayStatics::GetPlayerController(this, 0));
}

TScriptInterface<IBattle> APokemonBattle::Initialize(TArray<TScriptInterface<IBattleSide>> &&SidesIn) {
    Sides = MoveTemp(SidesIn);
    return this;
}

void APokemonBattle::JumpToBattleScene_Implementation(APlayerController *PlayerController) {
    Phase = EBattlePhase::Setup;
    check(BattlePawn != nullptr && PlayerController != nullptr)
    StoredPlayerPawn = PlayerController->GetPawnOrSpectator();
    PlayerController->Possess(BattlePawn);
    PlayBattleIntro();
}

void APokemonBattle::Tick(float DeltaSeconds) {
    using enum EBattlePhase;
    Super::Tick(DeltaSeconds);

    if (Phase == Selecting && ActionSelectionFinished()) {
        BeginActionProcessing();
    } else if (Phase == Actions) {
        if (ActionQueue.IsEmpty()) {
            Phase = Judging;
        } else if (auto Action = ActionQueue.Peek()->Get(); !Action->IsExecuting()) {
            if (Action->CanExecute()) {
                bActionMessagesDisplayed = false;
                bActionResultDisplaying = false;
                DisplayAction(Action->GetActionMessage());
            } else {
                ActionQueue.Pop();
            }
        } else if (bActionMessagesDisplayed) {
                Action->Execute();
        } else if (ActionQueue.Peek()->Get()->IsComplete()) {
            RefreshBattleHUD();
            NextAction();
        }
    } else if (Phase == Judging) {
        EndTurn();
    }
}

void APokemonBattle::StartBattle() {
    CreateBattleHUD();
    StartTurn();
}

void APokemonBattle::QueueAction(TUniquePtr<IBattleAction> &&Action) {
    auto &Battler = Action->GetBattler();
    auto BattlerId = Battler->GetInternalId();
    auto &ActionCount = CurrentActionCount.FindChecked(BattlerId);
    if (ExpectedActionCount.FindChecked(BattlerId) <= ActionCount) {
        UE_LOG(LogTemp, Error, TEXT("%s attempted to queue an action, but is already at capacity!"),
               *Battler->GetNickname().ToString())
        return;
    }

    FScopeLock Lock(&ActionMutex);
    SelectedActions.Add(MoveTemp(Action));
    ActionCount++;
}

bool APokemonBattle::ActionSelectionFinished() const {
    return Algo::NoneOf(ExpectedActionCount,
                        [this](const TPair<FGuid, uint8> &Pair) { return CurrentActionCount[Pair.Key] < Pair.Value; });
}

#if WITH_EDITOR
const TArray<TUniquePtr<IBattleAction>> & APokemonBattle::GetActions() const {
    return SelectedActions;
}

const TQueue<TUniquePtr<IBattleAction>> & APokemonBattle::GetActionQueue() const {
    return ActionQueue;
}


#endif

ranges::any_view<TScriptInterface<IBattleSide>> APokemonBattle::GetSides() const {
    return RangeHelpers::CreateRange(Sides);
}

ranges::any_view<TScriptInterface<IBattler>> APokemonBattle::GetActiveBattlers() const {
    return RangeHelpers::CreateRange(Sides) | ranges::views::transform(&GetBattlers) | ranges::views::join |
           ranges::views::filter(&IsNotFainted);
}

void APokemonBattle::ExecuteAction(IBattleAction &Action) {
    DisplayAction(Action.GetActionMessage());
}

APawn *APokemonBattle::GetBattlePawn() const {
    return BattlePawn;
}

void APokemonBattle::DisplayBattleIntroMessage() {
    check(Sides.IsValidIndex(1))
    ProcessBattleIntroMessage(Sides[1]->GetIntroText());
}

void APokemonBattle::OpponentSendOut() {
    check(Sides.IsValidIndex(1))
    const auto &Side = Sides[1];
    if (auto &SendOutText = Side->GetSendOutText(); SendOutText.IsSet()) {
        ProcessOpponentSendOutMessage(SendOutText.GetValue());
    } else {
        ProcessOpponentSendOutAnimation(Side);
    }
}

void APokemonBattle::OpponentSendOutAnimation() {
    check(Sides.IsValidIndex(1))
    const auto &Side = Sides[1];
    ProcessOpponentSendOutAnimation(Side);
}

void APokemonBattle::PlayerSendOut() {
    check(Sides.IsValidIndex(0))
    const auto &Side = Sides[0];
    if (auto &SendOutText = Side->GetSendOutText(); SendOutText.IsSet()) {
        ProcessPlayerSendOutMessage(SendOutText.GetValue());
    } else {
        ProcessPlayerSendOutAnimation(Side);
    }
}

void APokemonBattle::PlayerSendOutAnimation() {
    check(Sides.IsValidIndex(0))
    const auto &Side = Sides[0];
    ProcessPlayerSendOutAnimation(Side);
}

void APokemonBattle::ExitBattleScene() const {
    auto PlayerController = BattlePawn->GetController();
    PlayerController->Possess(StoredPlayerPawn);
}

void APokemonBattle::StartTurn() {
    TurnCount++;
    ExpectedActionCount.Reset();
    CurrentActionCount.Reset();
    Phase = EBattlePhase::Selecting;
    std::ranges::for_each(GetActiveBattlers(), [this](const TScriptInterface<IBattler> &Battler) {
        auto BattlerId = Battler->GetInternalId();
        CurrentActionCount.Add(BattlerId, 0);
        ExpectedActionCount.Add(BattlerId, Battler->GetActionCount());
        Battler->SelectActions();
    });
}

void APokemonBattle::EndTurn() {
    for (int32 i = 0; i < Sides.Num(); i++) {
        if (!Sides[i]->CanBattle()) {
            DecideBattle(i);
            return;
        }
    }
    StartTurn();
}

void APokemonBattle::BeginActionProcessing() {
    Phase = EBattlePhase::Actions;
    SelectedActions.Sort([](const TUniquePtr<IBattleAction> &A, const TUniquePtr<IBattleAction> &B) {
        if (A->GetPriority() > B->GetPriority()) {
            return true;
        }

        int32 SpeedA = A->GetBattler()->GetSpeed().GetModifiedValue();
        int32 SpeedB = B->GetBattler()->GetSpeed().GetModifiedValue();
        if (SpeedA == SpeedB) {
            return FMath::RandBool();
        }

        return SpeedA > SpeedB;
    });

    for (auto &Action : SelectedActions) {
        ActionQueue.Enqueue(MoveTemp(Action));
    }
    SelectedActions.Reset();
}

void APokemonBattle::NextAction() {
    ActionQueue.Pop();
}

void APokemonBattle::DecideBattle(int32 SideIndex) {
    Phase = EBattlePhase::Decided;
    if (SideIndex == 0) {
        ProcessPlayerLoss(false);
    } else {
        ProcessPlayerVictory(false);
    }
}