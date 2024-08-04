// "Unreal Pokémon" created by Retro & Chill.

#include "Battle/PokemonBattle.h"
#include "Algo/AnyOf.h"
#include "Algo/ForEach.h"
#include "Algo/NoneOf.h"
#include "Battle/Actions/BattleAction.h"
#include "Battle/Attributes/PokemonCoreAttributeSet.h"
#include "Battle/BattleAbilitySystemComponent.h"
#include "Battle/Battlers/Battler.h"
#include "Battle/Battlers/BattlerAbilityComponent.h"
#include "Battle/BattleSide.h"
#include "Battle/Events/BattleMessagePayload.h"
#include "Battle/Events/TargetedEvents.h"
#include "Battle/Tags.h"
#include "Battle/Transitions/BattleInfo.h"
#include "Battle/Transitions/BattleTransitionSubsystem.h"
#include "Chaos/ChaosPerfTest.h"
#include "Lookup/InjectionUtilities.h"
#include "Managers/PokemonSubsystem.h"
#include "Pokemon/Pokemon.h"
#include "range/v3/view/join.hpp"
#include "range/v3/view/transform.hpp"
#include "RangeHelpers.h"
#include <functional>
#include <range/v3/algorithm/for_each.hpp>
#include <range/v3/numeric/accumulate.hpp>
#include <range/v3/view/concat.hpp>
#include <range/v3/view/empty.hpp>
#include <range/v3/view/filter.hpp>

static auto GetBattlers(const TScriptInterface<IBattleSide> &Side) {
    return RangeHelpers::CreateRange(Side->GetBattlers());
}

static bool IsFainted(const TScriptInterface<IBattler> &Battler) {
    return Battler->IsFainted();
}

static bool IsNotFainted(const TScriptInterface<IBattler> &Battler) {
    return !Battler->IsFainted();
}

APokemonBattle::APokemonBattle() {
    AbilitySystemComponent = CreateDefaultSubobject<UBattleAbilitySystemComponent>(FName("AbilitySystemComponent"));
}

TScriptInterface<IBattle> APokemonBattle::Initialize(TArray<TScriptInterface<IBattleSide>> &&SidesIn) {
    Sides = MoveTemp(SidesIn);
    return this;
}

TScriptInterface<IBattle> APokemonBattle::Initialize(const FBattleInfo &BattleInfo) {
    TScriptInterface<IBattle> Self = this;
    Sides.Emplace(BattleInfo.CreatePlayerSide(Self, BattleSideClass.LoadSynchronous(), GetPlayerSidePosition()));
    Sides.Emplace(BattleInfo.CreateOpposingSide(Self, BattleSideClass.LoadSynchronous(), GetOpponentSidePosition()));
    Execute_JumpToBattleScene(this, GetWorld()->GetGameInstance()->GetPrimaryPlayerController(false));
    return Self;
}

void APokemonBattle::BeginPlay() {
    Super::BeginPlay();
    AbilitySystemComponent->InitAbilityActorInfo(this, this);
    auto TransitionSubsystem = GetWorld()->GetSubsystem<UBattleTransitionSubsystem>();
    check(TransitionSubsystem != nullptr)
    TransitionSubsystem->SetRegisteredBattle(this);
}

void APokemonBattle::EndPlay(const EEndPlayReason::Type EndPlayReason) {
    Super::EndPlay(EndPlayReason);
    auto AllSides =
        RangeHelpers::CreateRange(Sides) | ranges::views::transform([](const TScriptInterface<IBattleSide> &Side) {
            return CastChecked<AActor>(Side.GetObject());
        });
    ranges::for_each(AllSides, [](AActor *Actor) { Actor->Destroy(); });
}

bool APokemonBattle::IsTrainerBattle_Implementation() const {
    check(Sides.IsValidIndex(OpponentSideIndex))
    return !Sides[OpponentSideIndex]->GetTrainers().IsEmpty();
}

void APokemonBattle::JumpToBattleScene_Implementation(APlayerController *PlayerController) {
    Phase = EBattlePhase::Setup;
    check(BattlePawn != nullptr && PlayerController != nullptr)
    StoredPlayerPawn = PlayerController->GetPawnOrSpectator();
    PlayerController->Possess(BattlePawn);
    PlayerController->EnableInput(PlayerController);
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
            if (bSwitchPrompting) {
                EndTurn();
            } else {
                // We don't want to send out this event twice
                auto Payload = NewObject<UBattleMessagePayload>();
                Pokemon::Battle::Events::SendOutBattleEvent(this, Payload, Pokemon::Battle::EndTurn);
                ProcessTurnEndMessages(Payload->Messages);
            }
        } else if (auto Action = ActionQueue.Peek()->Get(); !Action->IsExecuting() && !bActionTextDisplayed) {
            if (Action->CanExecute()) {
                DisplayAction(Action->GetActionMessage());
                bActionTextDisplayed = true;
            } else {
                ActionQueue.Pop();
            }
        } else if (ActionQueue.Peek()->Get()->IsComplete()) {
            RefreshBattleHUD();
            NextAction();
        }
    }
}

void APokemonBattle::StartBattle() {
    CreateBattleHUD();
    OnBattlersEnteringBattle(GetActiveBattlers());
    StartTurn();
}

FRunningMessageSet APokemonBattle::OnBattlersEnteringBattle(ranges::any_view<TScriptInterface<IBattler>> Battlers) {
    FRunningMessageSet Messages;
    auto Sorted =
        Battlers | ranges::views::filter(&IsNotFainted) | RangeHelpers::TToArray<TScriptInterface<IBattler>>();
    Sorted.Sort([](const TScriptInterface<IBattler> &A, const TScriptInterface<IBattler> &B) {
        int32 SpeedA = FMath::FloorToInt32(A->GetAbilityComponent()->GetCoreAttributes()->GetSpeed());
        int32 SpeedB = FMath::FloorToInt32(B->GetAbilityComponent()->GetCoreAttributes()->GetSpeed());
        if (SpeedA == SpeedB) {
            return FMath::RandBool();
        }

        return SpeedA > SpeedB;
    });

    for (auto &Battler : Sorted) {
        Battler->RecordParticipation();
    }

    return Messages;
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
const TArray<TUniquePtr<IBattleAction>> &APokemonBattle::GetActions() const {
    return SelectedActions;
}

const TQueue<TUniquePtr<IBattleAction>> &APokemonBattle::GetActionQueue() const {
    return ActionQueue;
}

#endif

const TScriptInterface<IBattleSide> &APokemonBattle::GetPlayerSide() const {
    check(Sides.IsValidIndex(PlayerSideIndex))
    return Sides[PlayerSideIndex];
}

const TScriptInterface<IBattleSide> &APokemonBattle::GetOpposingSide() const {
    check(Sides.IsValidIndex(OpponentSideIndex))
    return Sides[OpponentSideIndex];
}

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

bool APokemonBattle::RunCheck_Implementation(const TScriptInterface<IBattler> &Battler, bool bDuringBattle) {
    if (!bDuringBattle) {
        RunAttempts++;
    }

    auto PlayerSpeed =
        Battler->GetAbilityComponent()->GetNumericAttributeBase(UPokemonCoreAttributeSet::GetSpeedAttribute());
    float EnemySpeed = 1.f;
    ranges::for_each(RangeHelpers::CreateRange(GetOpposingSide()->GetBattlers()) |
                         ranges::views::filter(&IsNotFainted) |
                         ranges::views::transform([](const TScriptInterface<IBattler> &Foe) {
                             return Foe->GetAbilityComponent()->GetNumericAttributeBase(
                                 UPokemonCoreAttributeSet::GetSpeedAttribute());
                         }),
                     [&EnemySpeed](float Speed) {
                         if (Speed > EnemySpeed) {
                             EnemySpeed = Speed;
                         }
                     });

    float Rate;
    if (PlayerSpeed > EnemySpeed) {
        Rate = 256.f;
    } else {
        Rate = PlayerSpeed * 128.f / EnemySpeed;
        Rate += static_cast<float>(RunAttempts * 30);
    }

    return Rate >= 256.f || FMath::Rand() % 256 < Rate;
}

void APokemonBattle::EndBattle_Implementation(EBattleResult Result) {
    Phase = EBattlePhase::Decided;
    ProcessBattleResult(Result);
}

void APokemonBattle::BindToOnBattleEnd(FOnBattleEnd::FDelegate &&Callback) {
    OnBattleEnd.Add(MoveTemp(Callback));
}

APawn *APokemonBattle::GetBattlePawn() const {
    return BattlePawn;
}

void APokemonBattle::DisplayBattleIntroMessage() {
    check(Sides.IsValidIndex(OpponentSideIndex))
    ProcessBattleIntroMessage(Sides[OpponentSideIndex]->GetIntroText());
}

void APokemonBattle::OpponentSendOut() {
    check(Sides.IsValidIndex(OpponentSideIndex))
    const auto &Side = Sides[OpponentSideIndex];
    if (auto &SendOutText = Side->GetSendOutText(); SendOutText.IsSet()) {
        ProcessOpponentSendOutMessage(SendOutText.GetValue());
    } else {
        ProcessOpponentSendOutAnimation(Side);
    }
}

void APokemonBattle::OpponentSendOutAnimation() {
    check(Sides.IsValidIndex(OpponentSideIndex))
    const auto &Side = Sides[OpponentSideIndex];
    ProcessOpponentSendOutAnimation(Side);
}

void APokemonBattle::PlayerSendOut() {
    check(Sides.IsValidIndex(PlayerSideIndex))
    const auto &Side = Sides[PlayerSideIndex];
    if (auto &SendOutText = Side->GetSendOutText(); SendOutText.IsSet()) {
        ProcessPlayerSendOutMessage(SendOutText.GetValue());
    } else {
        ProcessPlayerSendOutAnimation(Side);
    }
}

void APokemonBattle::PlayerSendOutAnimation() {
    check(Sides.IsValidIndex(PlayerSideIndex))
    const auto &Side = Sides[PlayerSideIndex];
    ProcessPlayerSendOutAnimation(Side);
}

void APokemonBattle::ExecuteAction() {
    check(!ActionQueue.IsEmpty())
    ActionQueue.Peek()->Get()->Execute();
}

void APokemonBattle::ExitBattleScene(EBattleResult Result) const {
    auto PlayerController = BattlePawn->GetController();
    PlayerController->Possess(StoredPlayerPawn);
    OnBattleEnd.Broadcast(Result);
}

void APokemonBattle::StartTurn() {
    bSwitchPrompting = false;
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
    ClearActionSelection();
    bool bRequiresSwaps = false;
    ExpectedActionCount.Reset();
    CurrentActionCount.Reset();
    for (int32 i = 0; i < Sides.Num(); i++) {
        if (!Sides[i]->CanBattle()) {
            DecideBattle(i);
            return;
        }

        // TODO: We need to determine what happens if you get damaged by an entry hazard and the Pokémon you sent out
        // faints
        ranges::for_each(RangeHelpers::CreateRange(Sides[i]->GetBattlers()) | ranges::views::filter(&IsFainted),
                         [this, &bRequiresSwaps](const TScriptInterface<IBattler> &Battler) {
                             auto BattlerId = Battler->GetInternalId();
                             CurrentActionCount.Add(BattlerId, 0);
                             ExpectedActionCount.Add(BattlerId, Battler->GetActionCount());
                             Battler->RequireSwitch();
                             bRequiresSwaps = true;
                         });
    }

    // If we need swaps, we're going to enter a second selecting and action phase to process the swaps
    if (bRequiresSwaps) {
        bSwitchPrompting = true;
        Phase = EBattlePhase::Selecting;
    } else {
        StartTurn();
    }
}

void APokemonBattle::BeginActionProcessing() {
    Phase = EBattlePhase::Actions;
    bActionTextDisplayed = false;
    SelectedActions.Sort([](const TUniquePtr<IBattleAction> &A, const TUniquePtr<IBattleAction> &B) {
        int32 PriorityA = A->GetPriority();
        int32 PriorityB = B->GetPriority();
        if (PriorityA > PriorityB) {
            return true;
        }
        if (PriorityA < PriorityB) {
            return false;
        }

        int32 SpeedA = FMath::FloorToInt32(A->GetBattler()->GetAbilityComponent()->GetCoreAttributes()->GetSpeed());
        int32 SpeedB = FMath::FloorToInt32(B->GetBattler()->GetAbilityComponent()->GetCoreAttributes()->GetSpeed());
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
    bActionTextDisplayed = false;
}

void APokemonBattle::DecideBattle(int32 SideIndex) {
    using enum EBattleResult;
    Execute_EndBattle(this, SideIndex == PlayerSideIndex ? Defeat : Victory);
}