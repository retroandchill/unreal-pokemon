// "Unreal Pokémon" created by Retro & Chill.


#include "Battle/PokemonBattle.h"
#include "Battle/BattleSide.h"
#include "Battle/Actions/BattleAction.h"
#include "Battle/Battlers/Battler.h"
#include "Lookup/InjectionUtilities.h"
#include "Mainpulation/RangeHelpers.h"
#include "Managers/PokemonSubsystem.h"
#include "Pokemon/Pokemon.h"

static auto GetBattlers(const TScriptInterface<IBattleSide> &Side) {
    return RangeHelpers::CreateRange(Side->GetBattlers());
}

void APokemonBattle::CreateWildBattle(const FPokemonDTO& Pokemon) {
    TScriptInterface<IBattle> Self = this;
    auto PlayerSide = GetWorld()->SpawnActor<AActor>(BattleSideClass.LoadSynchronous(), GetPlayerSidePosition());
    Sides.Emplace_GetRef(PlayerSide)->Initialize(Self, UPokemonSubsystem::GetInstance(this).GetPlayer(), BattleSettings.BattlefieldSize.PlayerSide, true);
    PlayerSide->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepWorld, true));
    
    auto OpponentSide = GetWorld()->SpawnActor<AActor>(BattleSideClass.LoadSynchronous(), GetOpponentSidePosition());
    Sides.Emplace_GetRef(OpponentSide)->Initialize(Self, UnrealInjector::NewInjectedDependency<IPokemon>(this, Pokemon));
    OpponentSide->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepWorld, true));
    IBattle::Execute_JumpToBattleScene(this, UGameplayStatics::GetPlayerController(this, 0));
}

TScriptInterface<IBattle> APokemonBattle::Initialize(TArray<TScriptInterface<IBattleSide>> &&SidesIn) {
    Sides = MoveTemp(SidesIn);
    return this;
}

void APokemonBattle::JumpToBattleScene_Implementation(APlayerController* PlayerController) {
    check(BattlePawn != nullptr && PlayerController != nullptr)
    StoredPlayerPawn = PlayerController->GetPawnOrSpectator();
    PlayerController->Possess(BattlePawn);
    PlayBattleIntro();
}

void APokemonBattle::StartBattle() {
    StartTurn();
}

void APokemonBattle::QueueAction(TUniquePtr<IBattleAction>&& Action) {
    auto &Battler = Action->GetBattler();
    auto BattlerId = Battler->GetInternalId();
    auto &ActionCount = CurrentActionCount.FindChecked(BattlerId);
    if (ExpectedActionCount.FindChecked(BattlerId) <= ActionCount) {
        UE_LOG(LogTemp, Error, TEXT("%s attempted to queue an action, but is already at capacity!"),
            *Battler->GetNickname().ToString());
        return;
    }
    
    FScopeLock Lock(&ActionMutex);
    ActionQueue.Add(MoveTemp(Action));
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

void APokemonBattle::ForEachActiveBattler(const TFunctionRef<void(const TScriptInterface<IBattler> &)> &Callback) const {
    std::ranges::for_each(RangeHelpers::CreateRange(Sides)
        | std::views::transform(&GetBattlers)
        | std::ranges::views::join, Callback);
}

void APokemonBattle::ForEachFieldEffect(const TFunctionRef<void(const TScriptInterface<IFieldEffect> &)> Callback) const {
}

bool APokemonBattle::FindGlobalAbility(FName AbilityID) const {
    return false;
}

APawn * APokemonBattle::GetBattlePawn() const {
    return BattlePawn;
}

void APokemonBattle::DisplayBattleIntroMessage() const {
}

void APokemonBattle::SetUpSides() {
    
}

void APokemonBattle::StartTurn() {
    TurnCount++;
    ExpectedActionCount.Reset();
    CurrentActionCount.Reset();
    ForEachActiveBattler([this](const TScriptInterface<IBattler>& Battler) {
            auto BattlerId = Battler->GetInternalId();
            CurrentActionCount.Add(BattlerId, 0);
            ExpectedActionCount.Add(BattlerId, Battler->GetActionCount());
            Battler->SelectActions();
        });
}