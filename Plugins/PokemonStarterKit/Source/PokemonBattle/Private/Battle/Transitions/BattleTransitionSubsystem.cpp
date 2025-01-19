// "Unreal Pokémon" created by Retro & Chill.

#include "Battle/Transitions/BattleTransitionSubsystem.h"
#include "Battle/Battle.h"
#include "Battle/Settings/PokemonBattleSettings.h"
#include "Battle/Transitions/BattleTransitionActor.h"
#include "Engine/LevelStreamingDynamic.h"
#include "Engine/LevelStreamingVolume.h"
#include "Kismet/GameplayStatics.h"
#include "Managers/PokemonSubsystem.h"

void UBattleTransitionSubsystem::Initialize(FSubsystemCollectionBase &Collection) {
    Super::Initialize(Collection);
    BattleMap = GetDefault<UPokemonBattleSettings>()->DefaultBattleScene;
}

void UBattleTransitionSubsystem::SetBattleMap(const TSoftObjectPtr<UWorld> &NewBattleMap) {
    BattleMap = NewBattleMap;
}

void UBattleTransitionSubsystem::SetRegisteredBattle(const TScriptInterface<IBattle> &Battle) {
    RegisteredBattle = Battle;
}

UE5Coro::TCoroutine<EBattleResult>
UBattleTransitionSubsystem::InitiateBattle(const FBattleInfo &Info, TSubclassOf<ABattleTransitionActor> Transition,
                                           FForceLatentCoroutine) {
    auto PlayerController = GetWorld()->GetGameInstance()->GetPrimaryPlayerController(false);
    PlayerController->GetPawn()->DisableInput(PlayerController);
    static auto &BattleLevelOffset = GetDefault<UPokemonBattleSettings>()->BattleSceneOffset;
    if (Transition != nullptr) {
        bBattleInitialized = false;
        CurrentTransition = GetWorld()->SpawnActor<ABattleTransitionActor>(Transition);
    }

    StreamingStates.Reset();
    TArray<AActor *> LevelStreamingVolumes;
    UGameplayStatics::GetAllActorsOfClass(this, ALevelStreamingVolume::StaticClass(), LevelStreamingVolumes);
    for (auto Actor : LevelStreamingVolumes) {
        auto &[Volume, bDisabled] = StreamingStates.Emplace_GetRef();
        Volume = CastChecked<ALevelStreamingVolume>(Actor);
        bDisabled = Volume->bDisabled;
        Volume->bDisabled = true;
    }

    bool bSuccess;
    Battlefield = ULevelStreamingDynamic::LoadLevelInstanceBySoftObjectPtr(this, BattleMap, BattleLevelOffset,
                                                                           FRotator(), bSuccess);
    check(bSuccess)
    BattleInfo.Emplace(Info);
    if (Battlefield->IsLevelVisible()) {
        co_await ABattleTransitionActor::Execute(CurrentTransition);
    } else {
        co_await WhenAll(UE5Coro::Latent::UntilDelegate(Battlefield->OnLevelShown),
                         ABattleTransitionActor::Execute(CurrentTransition));
    }

    if (RegisteredBattle.IsValid() && !bBattleInitialized) {
        RegisteredBattle->Initialize(BattleInfo.GetValue());
        bBattleInitialized = true;
    }

    CurrentTransition = nullptr;
    SetUpBattle();
    auto Result =
        co_await RegisteredBattle->ConductBattle(GetWorld()->GetGameInstance()->GetPrimaryPlayerController(false));
    co_await ExitBattle();
    co_return Result;
}

FDelegateHandle UBattleTransitionSubsystem::BindToBattleFinished(FBattleFinished::FDelegate &&Callback) {
    return BattleFinished.Add(std::move(Callback));
}

void UBattleTransitionSubsystem::RemoveFromBattleFinished(const FDelegateHandle &Handle) {
    BattleFinished.Remove(Handle);
}

void UBattleTransitionSubsystem::SetUpBattle() {
    check(RegisteredBattle.IsValid())
    check(BattleInfo.IsSet())

    if (CurrentTransition == nullptr && !bBattleInitialized) {
        RegisteredBattle->Initialize(BattleInfo.GetValue());
        bBattleInitialized = true;
    }
}

UE5Coro::TCoroutine<> UBattleTransitionSubsystem::ExitBattle(FForceLatentCoroutine) {
    check(Battlefield != nullptr)
    FLatentActionInfo LatentActionInfo;
    UGameplayStatics::UnloadStreamLevelBySoftObjectPtr(this, Battlefield->GetWorldAsset(), LatentActionInfo, false);
    check(BattleInfo.IsSet())
    BattleInfo.Reset();

    for (const auto &[Volume, bDisabled] : StreamingStates) {
        Volume->bDisabled = bDisabled;
    }
    StreamingStates.Reset();
    bBattleInitialized = false;

    auto PlayerController = GetWorld()->GetGameInstance()->GetPrimaryPlayerController(false);
    PlayerController->GetPawn()->EnableInput(PlayerController);

    co_await Battlefield->OnLevelUnloaded;
    Battlefield = nullptr;
}