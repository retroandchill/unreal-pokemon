// "Unreal Pokémon" created by Retro & Chill.

#include "Battle/PokemonBattle.h"
#include "Algo/NoneOf.h"
#include "Battle/Actions/BattleAction.h"
#include "Battle/Animations/BattleAnimation.h"
#include "Battle/Animations/BattleSequencer.h"
#include "Battle/Attributes/PokemonCoreAttributeSet.h"
#include "Battle/BattleAbilitySystemComponent.h"
#include "Battle/Battlers/Battler.h"
#include "Battle/Battlers/BattlerAbilityComponent.h"
#include "Battle/BattleSide.h"
#include "Battle/Display/BattleHUD.h"
#include "Battle/Effects/TurnBasedEffectComponent.h"
#include "Battle/Events/TargetedEvents.h"
#include "Battle/Tags.h"
#include "Battle/Transitions/BattleInfo.h"
#include "Battle/Transitions/BattleTransitionSubsystem.h"
#include "RetroLib/Casting/DynamicCast.h"
#include "RetroLib/Optionals/IfPresent.h"
#include "RetroLib/Optionals/Transform.h"
#include "RetroLib/Ranges/Algorithm/NameAliases.h"
#include "RetroLib/Ranges/Algorithm/To.h"
#include "RetroLib/Ranges/Views/Concat.h"
#include "Utilities/PokemonCoroutineDispatcher.h"

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
    return Self;
}

void APokemonBattle::BeginPlay() {
    Super::BeginPlay();
    BattleSequencer = GetWorld()->SpawnActor<ABattleSequencer>(BattleSequencerClass.LoadSynchronous());
    AbilitySystemComponent->InitAbilityActorInfo(this, this);
    auto TransitionSubsystem = GetWorld()->GetSubsystem<UBattleTransitionSubsystem>();
    check(TransitionSubsystem != nullptr)
    TransitionSubsystem->SetRegisteredBattle(this);
    Dispatcher = Retro::WrapPointer(&IPokemonCoroutineDispatcher::Get(this));
}

void APokemonBattle::EndPlay(const EEndPlayReason::Type EndPlayReason) {
    Super::EndPlay(EndPlayReason);
    // clang-format off
    Sides |
        Retro::Ranges::Views::Transform(Retro::DynamicCastChecked<AActor>) |
        Retro::Ranges::ForEach([](AActor *Actor) {
            Actor->Destroy();
        });
    // clang-format on
    BattleSequencer->Destroy();
}

bool APokemonBattle::IsTrainerBattle_Implementation() const {
    check(Sides.IsValidIndex(OpponentSideIndex))
    return !Sides[OpponentSideIndex]->GetTrainers().IsEmpty();
}

UE5Coro::TCoroutine<> APokemonBattle::DisplaySideSendOutAnimation(int32 Index) {
    check(Sides.IsValidIndex(Index))
    if (auto SendOutText = Sides[Index]->GetSendOutText(); SendOutText.IsSet()) {
        co_await Dispatcher->DisplayMessage(std::move(*SendOutText));
    }
    co_await IBattleAnimation::PlayAnimation(Index == OpponentSideIndex ? GetOpponentSendOutAnimation()
                                                                        : GetPlayerSendOutAnimation());
}

UE5Coro::TCoroutine<EBattleResult> APokemonBattle::MainBattleLoop() {
    TOptional<int32> Result;
    while (true) {
        Result = co_await ProcessTurn();
        if (Result.IsSet()) {
            break;
        }
    }

    check(Result.IsSet())
    co_return co_await DecideBattle(*Result);
}

UE5Coro::TCoroutine<EBattleResult> APokemonBattle::ConductBattle(APlayerController *PlayerController,
                                                                 FForceLatentCoroutine) {

    Phase = EBattlePhase::Setup;
    check(BattlePawn != nullptr && PlayerController != nullptr)
    StoredPlayerPawn = PlayerController->GetPawnOrSpectator();
    PlayerController->Possess(BattlePawn);
    co_await IBattleAnimation::PlayAnimation(GetBattleIntro());
    co_await Dispatcher->DisplayMessage(GetBattleIntroMessage());
    co_await DisplaySideSendOutAnimation(OpponentSideIndex);
    co_await DisplaySideSendOutAnimation(PlayerSideIndex);

    co_await StartBattle();

    auto MainLoop = MainBattleLoop();
    auto Interrupt = [](UE5Coro::TLatentContext<APokemonBattle> This) -> UE5Coro::TCoroutine<EBattleResult> {
        co_return co_await TFuture<EBattleResult>(This->OnBattleEnd);
    }(this);
    auto Result = co_await Race(MainLoop, Interrupt) == 0 ? MainLoop.GetResult() : Interrupt.GetResult();

    co_await IBattleAnimation::PlayAnimation(GetBattleEndAnimation(Result));

    PlayerController->Possess(StoredPlayerPawn);
    co_return Result;
}

UE5Coro::TCoroutine<> APokemonBattle::StartBattle() {
    HUD = CreateBattleHUD();
    co_await OnBattlersEnteringBattle(GetActiveBattlers());
}

UE5Coro::TCoroutine<>
APokemonBattle::OnBattlersEnteringBattle(Retro::Ranges::TAnyView<TScriptInterface<IBattler>> Battlers) {
    // clang-format off
    auto Sorted = std::move(Battlers) |
                  Retro::Ranges::Views::Filter(&IBattler::IsNotFainted) |
                  Retro::Ranges::To<TArray>();
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

    // TODO: We need to actually handle the battle entrance abilities
    co_return;
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
    if (ActionSelectionFinished()) {
        ActionsCompletePromise->EmplaceResult(ActionCount);
    }
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

Retro::TGenerator<TScriptInterface<IBattleSide>> APokemonBattle::GetSides() const {
    co_yield Retro::Ranges::TElementsOf(Sides);
}

Retro::TGenerator<TScriptInterface<IBattler>> APokemonBattle::GetActiveBattlers() const {
    // clang-format off
    co_yield Retro::Ranges::TElementsOf(Sides |
           Retro::Ranges::Views::Transform(&IBattleSide::GetBattlers) |
           Retro::Ranges::Views::Join |
           Retro::Ranges::Views::Filter(&IBattler::IsNotFainted));
    // clang-format on
}

UE5Coro::TCoroutine<> APokemonBattle::ExecuteAction(IBattleAction &Action, FForceLatentCoroutine) {
    if (auto Message = Action.GetActionMessage(); !Message.IsEmptyOrWhitespace()) {
        co_await Dispatcher->DisplayMessage(std::move(Message));
    }
    co_await Action.Execute();
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
        Retro::Ranges::Views::Filter(&IBattler::IsNotFainted) |
        Retro::Ranges::Views::Transform(&IBattler::GetAbilityComponent) |
        Retro::Ranges::Views::Transform(Retro::BindBack<&UAbilitySystemComponent::GetNumericAttributeBase>(UPokemonCoreAttributeSet::GetSpeedAttribute())) |
        Retro::Ranges::ForEach([&EnemySpeed](float Speed) {
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

APawn *APokemonBattle::GetBattlePawn() const {
    return BattlePawn;
}

FText APokemonBattle::GetBattleIntroMessage() const {
    check(Sides.IsValidIndex(OpponentSideIndex))
    return Sides[OpponentSideIndex]->GetIntroText();
}

void APokemonBattle::EndBattle_Implementation(EBattleResult Result) {
    if (!OnBattleEnd->IsComplete()) {
        OnBattleEnd->EmplaceResult(Result);
    }
}

void APokemonBattle::ProcessTurnDurationTrigger(ETurnDurationTrigger Trigger) {
    // clang-format off
    auto MyComponent = Retro::Ranges::Views::Single(TurnBasedEffectComponent.Get());
    auto ChildComponents = Sides |
                           Retro::Ranges::Views::Transform(&IBattleSide::GetChildEffectComponents) |
                           Retro::Ranges::Views::Join;
    Retro::Ranges::Views::Concat(std::move(MyComponent), std::move(ChildComponents)) |
        Retro::Ranges::Views::Transform(Retro::BindBack<&UTurnBasedEffectComponent::GetAllTurnBasedEffectsForTrigger>(Trigger)) |
        Retro::Ranges::Views::Join |
        Retro::Ranges::ForEach(&FTurnBasedGameplayEffect::IncrementTurnCount);
    // clang-format on
}

UE5Coro::TCoroutine<TOptional<int32>> APokemonBattle::ProcessTurn() {
    bSwitchPrompting = false;
    TurnCount++;

    ExpectedActionCount.Reset();
    CurrentActionCount.Reset();
    ActionsCompletePromise = MakeShared<TFutureState<int32>>();

    for (auto Battler : GetActiveBattlers()) {
        auto BattlerId = Battler->GetInternalId();
        CurrentActionCount.Add(BattlerId, 0);
        ExpectedActionCount.Add(BattlerId, Battler->GetActionCount());
        Battler->SelectActions();
    }

    co_await TFuture<void>(ActionsCompletePromise);

    co_await ActionProcessing();

    co_return co_await EndTurn();
}

UE5Coro::TCoroutine<TOptional<int32>> APokemonBattle::EndTurn() {
    ClearActionSelection();
    bool bRequiresSwaps = false;
    ExpectedActionCount.Reset();
    CurrentActionCount.Reset();
    ActionsCompletePromise = MakeShared<TFutureState<int32>>();
    for (int32 i = 0; i < Sides.Num(); i++) {
        if (!Sides[i]->CanBattle()) {
            co_return i;
        }

        // TODO: We need to determine what happens if you get damaged by an entry hazard and the Pokémon you sent out
        // faints
        for (auto Battler : Sides[i]->GetBattlers() | Retro::Ranges::Views::Filter(&IBattler::IsFainted)) {
            auto BattlerId = Battler->GetInternalId();
            CurrentActionCount.Add(BattlerId, 0);
            ExpectedActionCount.Add(BattlerId, Battler->GetActionCount());
            Battler->RequireSwitch();
            bRequiresSwaps = true;
        }
    }

    if (bRequiresSwaps) {
        bSwitchPrompting = true;
        co_await TFuture<void>(ActionsCompletePromise);
        co_await ActionProcessing();
    }

    ProcessTurnDurationTrigger(ETurnDurationTrigger::TurnEnd);
    co_await Pokemon::Battle::Events::SendOutBattleEvent(this, nullptr, Pokemon::Battle::EndTurn, {});
    co_await ABattleSequencer::DisplayBattleMessages(this);

    co_return {};
}

UE5Coro::TCoroutine<> APokemonBattle::ActionProcessing() {
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

    while (!ActionQueue.IsEmpty()) {
        if (auto Action = ActionQueue.Peek()->Get(); Action->CanExecute()) {
            co_await ExecuteAction(*Action);
        }

        ActionQueue.Pop();
    }
}

void APokemonBattle::NextAction() {
    ActionQueue.Pop();
    bActionTextDisplayed = false;
}

UE5Coro::TCoroutine<EBattleResult> APokemonBattle::DecideBattle(int32 SideIndex) {
    using enum EBattleResult;
    auto Result = SideIndex == PlayerSideIndex ? Defeat : Victory;
    co_await IBattleAnimation::PlayAnimation(GetBattleResultAnimation(Result));
    co_return Result;
}