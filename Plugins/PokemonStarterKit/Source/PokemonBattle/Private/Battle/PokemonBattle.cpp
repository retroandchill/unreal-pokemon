// "Unreal Pokémon" created by Retro & Chill.

#include "Battle/PokemonBattle.h"
#include "Algo/NoneOf.h"
#include "Battle/Actions/BattleAction.h"
#include "Battle/Attributes/PokemonCoreAttributeSet.h"
#include "Battle/BattleAbilitySystemComponent.h"
#include "Battle/Battlers/Battler.h"
#include "Battle/Battlers/BattlerAbilityComponent.h"
#include "Battle/BattleSide.h"
#include "Battle/Events/TargetedEvents.h"
#include "Battle/Tags.h"
#include "Battle/Animations/BattleSequencer.h"
#include "Battle/Effects/TurnBasedEffectComponent.h"
#include "Battle/Transitions/BattleInfo.h"
#include "Battle/Transitions/BattleTransitionSubsystem.h"
#include "Pokemon/Pokemon.h"
#include "range/v3/view/join.hpp"
#include "Ranges/Algorithm/ForEach.h"
#include "Ranges/Algorithm/ToArray.h"
#include "Ranges/Casting/DynamicCast.h"
#include "Ranges/Optional/IfPresent.h"
#include "Ranges/Optional/Map.h"
#include "Ranges/Views/Concat.h"
#include "Ranges/Views/ContainerView.h"
#include "Ranges/Views/Filter.h"
#include "Ranges/Views/Join.h"
#include "Ranges/Views/Single.h"

APokemonBattle::APokemonBattle() {
    AbilitySystemComponent = CreateDefaultSubobject<UBattleAbilitySystemComponent>(FName("AbilitySystemComponent"));
    TurnBasedEffectComponent = CreateDefaultSubobject<UTurnBasedEffectComponent>(FName("TurnBasedEffectsComponent"));
}

TScriptInterface<IBattle> APokemonBattle::Initialize(TArray<TScriptInterface<IBattleSide>> &&SidesIn) {
    Sides = std::move(SidesIn);
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
    BattleSequencer = GetWorld()->SpawnActor<ABattleSequencer>(BattleSequencerClass.LoadSynchronous());
    AbilitySystemComponent->InitAbilityActorInfo(this, this);
    auto TransitionSubsystem = GetWorld()->GetSubsystem<UBattleTransitionSubsystem>();
    check(TransitionSubsystem != nullptr)
    TransitionSubsystem->SetRegisteredBattle(this);
}

void APokemonBattle::EndPlay(const EEndPlayReason::Type EndPlayReason) {
    Super::EndPlay(EndPlayReason);
    // clang-format off
    Sides |
        UE::Ranges::Map(UE::Ranges::DynamicCastChecked<AActor>) |
        UE::Ranges::ForEach([](AActor *Actor) {
            Actor->Destroy();
        });
    // clang-format on
    BattleSequencer->Destroy();
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
    QueueBattleIntro();
    QueueOpponentSendOut();
    QueuePlayerSendOut();
    ABattleSequencer::DisplayBattleMessages(this, &APokemonBattle::StartBattle);
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
                ProcessTurnDurationTrigger(ETurnDurationTrigger::TurnEnd);
                Pokemon::Battle::Events::SendOutBattleEvent(this, nullptr, Pokemon::Battle::EndTurn);
                ABattleSequencer::DisplayBattleMessages(this, &APokemonBattle::EndTurn);
            }
        } else if (auto Action = ActionQueue.Peek()->Get(); !Action->IsExecuting() && !bActionTextDisplayed) {
            if (Action->CanExecute()) {
                ExecuteAction(*Action);
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
    ABattleSequencer::DisplayBattleMessages(this, &APokemonBattle::StartTurn);
}

void APokemonBattle::OnBattlersEnteringBattle(UE::Ranges::TAnyView<TScriptInterface<IBattler>> Battlers) {
    // clang-format off
    auto Sorted = Battlers |
                  UE::Ranges::Filter(&IBattler::IsNotFainted) |
                  UE::Ranges::ToArray;
    // clang-format on
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
    SelectedActions.Add(std::move(Action));
    ActionCount++;
}

bool APokemonBattle::ActionSelectionFinished() const {
    return Algo::NoneOf(ExpectedActionCount,
                        [this](const TPair<FGuid, uint8> &Pair) {
                            return CurrentActionCount[Pair.Key] < Pair.Value;
                        });
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

UE::Ranges::TAnyView<TScriptInterface<IBattleSide>> APokemonBattle::GetSides() const {
    return UE::Ranges::CreateRange(Sides);
}

UE::Ranges::TAnyView<TScriptInterface<IBattler>> APokemonBattle::GetActiveBattlers() const {
    // clang-format off
    return Sides |
           UE::Ranges::Map(&IBattleSide::GetBattlers) |
           UE::Ranges::Join |
           UE::Ranges::Filter(&IBattler::IsNotFainted);
    // clang-format on
}

void APokemonBattle::ExecuteAction(IBattleAction &Action) {
    bActionTextDisplayed = true;
    QueueDisplayAction(Action.GetActionMessage());
    ABattleSequencer::DisplayBattleMessages(this, [this] {
        ExecuteAction();
    });
}

bool APokemonBattle::RunCheck_Implementation(const TScriptInterface<IBattler> &Battler, bool bDuringBattle) {
    if (!bDuringBattle) {
        RunAttempts++;
    }

    auto PlayerSpeed =
        Battler->GetAbilityComponent()->GetNumericAttributeBase(UPokemonCoreAttributeSet::GetSpeedAttribute());
    float EnemySpeed = 1.f;
    // clang-format off
    GetOpposingSide()->GetBattlers() |
        UE::Ranges::Filter(&IBattler::IsNotFainted) |
        UE::Ranges::Map(&IBattler::GetAbilityComponent) |
        UE::Ranges::Map(&UAbilitySystemComponent::GetNumericAttributeBase,
                        UPokemonCoreAttributeSet::GetSpeedAttribute()) |
        UE::Ranges::ForEach([&EnemySpeed](float Speed) {
            if (Speed > EnemySpeed) {
                EnemySpeed = Speed;
            }
        });
    // clang-format on

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
    QueueBattleResultAnimation(Result);
    ABattleSequencer::DisplayBattleMessages(this, &APokemonBattle::ExitBattleScene, Result);
}

void APokemonBattle::BindToOnBattleEnd(FOnBattleEnd::FDelegate &&Callback) {
    OnBattleEnd.Add(std::move(Callback));
}

void APokemonBattle::ClearOnBattleEnd() {
    OnBattleEnd.Clear();
}

APawn *APokemonBattle::GetBattlePawn() const {
    return BattlePawn;
}

FText APokemonBattle::GetBattleIntroMessage() const {
    check(Sides.IsValidIndex(OpponentSideIndex))
    return Sides[OpponentSideIndex]->GetIntroText();
}

void APokemonBattle::QueueOpponentSendOut() {
    check(Sides.IsValidIndex(OpponentSideIndex))
    const auto &Side = Sides[OpponentSideIndex];
    QueueOpponentSendOutMessage(Side->GetSendOutText().Get(FText::GetEmpty()));
}

void APokemonBattle::QueuePlayerSendOut() {
    check(Sides.IsValidIndex(PlayerSideIndex))
    const auto &Side = Sides[PlayerSideIndex];
    QueuePlayerSendOutMessage(Side->GetSendOutText().Get(FText::GetEmpty()));
}

void APokemonBattle::ExecuteAction() {
    check(!ActionQueue.IsEmpty())
    ActionQueue.Peek()->Get()->Execute();
}

void APokemonBattle::ExitBattleScene(EBattleResult Result) const {
    // clang-format off
    UE::Optionals::OfNullable(BattlePawn) |
        UE::Optionals::Map([](const APawn *Pawn) { return Pawn->GetController(); }) |
        UE::Optionals::IfPresent(&APlayerController::Possess, StoredPlayerPawn);
    // clang-format on
    OnBattleEnd.Broadcast(Result);
}

void APokemonBattle::ProcessTurnDurationTrigger(ETurnDurationTrigger Trigger) {
    // clang-format off
    auto MyComponent = UE::Ranges::Single(TurnBasedEffectComponent.Get());
    auto ChildComponents = Sides |
                           UE::Ranges::Map(&IBattleSide::GetChildEffectComponents) |
                           UE::Ranges::Join;
    UE::Ranges::Concat(std::move(MyComponent), std::move(ChildComponents)) |
        UE::Ranges::Map(&UTurnBasedEffectComponent::GetAllTurnBasedEffectsForTrigger, Trigger) |
        UE::Ranges::Join |
        UE::Ranges::ForEach(&FTurnBasedGameplayEffect::IncrementTurnCount);
    // clang-format on
}

void APokemonBattle::StartTurn() {
    bSwitchPrompting = false;
    TurnCount++;

    ExpectedActionCount.Reset();
    CurrentActionCount.Reset();
    Phase = EBattlePhase::Selecting;
    GetActiveBattlers() | UE::Ranges::ForEach([this](const TScriptInterface<IBattler> &Battler) {
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
        Sides[i]->GetBattlers() | UE::Ranges::Filter(&IBattler::IsFainted) |
            UE::Ranges::ForEach([this, &bRequiresSwaps](const TScriptInterface<IBattler> &Battler) {
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
        ActionQueue.Enqueue(std::move(Action));
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