// "Unreal Pokémon" created by Retro & Chill.

#include "Battle/PokemonBattle.h"
#include "Battle/Actions/BattleAction.h"
#include "Battle/Battlers/Battler.h"
#include "Battle/BattleSide.h"
#include "Lookup/InjectionUtilities.h"
#include "Mainpulation/RangeHelpers.h"
#include "Managers/PokemonSubsystem.h"
#include "Pokemon/Pokemon.h"
#include <functional>

static auto GetBattlers(const TScriptInterface<IBattleSide> &Side) {
    return RangeHelpers::CreateRange(Side->GetBattlers());
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
            bActionMessagesDisplayed = false;
            DisplayAction(Action->GetActionMessage());
            Action->Execute();
        } else if (auto &ResultFuture = ActionQueue.Peek()->Get()->GetActionResult(); bActionMessagesDisplayed && ResultFuture.IsReady()) {
            auto Result = ResultFuture.Consume();
            for (auto &[Target, bHit, Damage] : Result.TargetResults) {
                Target->TakeBattleDamage(Damage.Damage);
            }

            RefreshBattleHUD();
            NextAction();
        }
    } else if (Phase == Judging) {
        // TODO: Actually judge the battle
        StartTurn();
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
               *Battler->GetNickname().ToString());
        return;
    }

    FScopeLock Lock(&ActionMutex);
    SelectedActions.Add(MoveTemp(Action));
    ActionCount++;
}

bool APokemonBattle::ActionSelectionFinished() const {
    for (auto [ID, Count] : ExpectedActionCount) {
        if (CurrentActionCount[ID] < Count) {
            return false;
        }
    }

    return true;
}

bool APokemonBattle::ShouldIgnoreAbilities() const {
    return false;
}

void APokemonBattle::ForEachSide(
    const TFunctionRef<void(int32, const TScriptInterface<IBattleSide> &)> &Callback) const {
    for (int32 i = 0; i < Sides.Num(); i++) {
        Callback(i, Sides[i]);
    }
}

void APokemonBattle::ForEachActiveBattler(
    const TFunctionRef<void(const TScriptInterface<IBattler> &)> &Callback) const {
    std::ranges::for_each(
        RangeHelpers::CreateRange(Sides) | std::views::transform(&GetBattlers) | std::ranges::views::join, Callback);
}

void APokemonBattle::ForEachFieldEffect(
    const TFunctionRef<void(const TScriptInterface<IFieldEffect> &)> Callback) const {
}

bool APokemonBattle::FindGlobalAbility(FName AbilityID) const {
    return false;
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

void APokemonBattle::ApplyActionResult() {
    bActionMessagesDisplayed = true;
}

void APokemonBattle::StartTurn() {
    TurnCount++;
    ExpectedActionCount.Reset();
    CurrentActionCount.Reset();
    Phase = EBattlePhase::Selecting;
    ForEachActiveBattler([this](const TScriptInterface<IBattler> &Battler) {
        auto BattlerId = Battler->GetInternalId();
        CurrentActionCount.Add(BattlerId, 0);
        ExpectedActionCount.Add(BattlerId, Battler->GetActionCount());
        Battler->SelectActions();
    });
}

void APokemonBattle::BeginActionProcessing() {
    Phase = EBattlePhase::Actions;
    SelectedActions.Sort([](const TUniquePtr<IBattleAction>& A, const TUniquePtr<IBattleAction>& B) {
        if (A->GetPriority() > B->GetPriority()) {
            return true;
        }

        int32 SpeedA = A->GetBattler()->GetSpeed();
        int32 SpeedB = B->GetBattler()->GetSpeed();
        if (SpeedA == SpeedB) {
            return FMath::RandBool();
        }
        
        return SpeedA > SpeedB;
    });
    
    for (auto& Action : SelectedActions) {
        Action->BindToActionFinished(FOnActionFinished::CreateLambda(std::bind_front(&APokemonBattle::NextAction, this)));
        ActionQueue.Enqueue(MoveTemp(Action));
    }
    SelectedActions.Reset();
    
}

void APokemonBattle::NextAction() {
    ActionQueue.Pop();
}