// "Unreal Pokémon" created by Retro & Chill.

#include "Battle/Transitions/BattleTransitionSubsystem.h"
#include "Battle/Battle.h"
#include "Battle/Settings/PokemonBattleSettings.h"
#include "Battle/Transitions/BattleTransitionActor.h"
#include "Engine/LevelStreamingDynamic.h"
#include "Engine/LevelStreamingVolume.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/GameInstance.h"
#include "GameFramework/Pawn.h"
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
UBattleTransitionSubsystem::InitiateBattle(FBattleInfo Info, TSubclassOf<ABattleTransitionActor> Transition,
                                           FForceLatentCoroutine) {
    auto PlayerController = GetWorld()->GetGameInstance()->GetPrimaryPlayerController(false);
    PlayerController->GetPawn()->DisableInput(PlayerController);
    static auto &BattleLevelOffset = GetDefault<UPokemonBattleSettings>()->BattleSceneOffset;
    ABattleTransitionActor *CurrentTransition = nullptr;
    if (Transition != nullptr) {
        CurrentTransition = GetWorld()->SpawnActor<ABattleTransitionActor>(Transition);
    }

    TArray<FLevelStreamingVolumeState> StreamingStates;
    TArray<AActor *> LevelStreamingVolumes;
    UGameplayStatics::GetAllActorsOfClass(this, ALevelStreamingVolume::StaticClass(), LevelStreamingVolumes);
    for (auto Actor : LevelStreamingVolumes) {
        auto &[Volume, bDisabled] = StreamingStates.Emplace_GetRef();
        Volume = CastChecked<ALevelStreamingVolume>(Actor);
        bDisabled = Volume->bDisabled;
        Volume->bDisabled = true;
    }

    bool bSuccess;
    ULevelStreamingDynamic *Battlefield = ULevelStreamingDynamic::LoadLevelInstanceBySoftObjectPtr(
        this, BattleMap, BattleLevelOffset, FRotator(), bSuccess);
    check(bSuccess)
    if (Battlefield->IsLevelVisible()) {
        co_await ABattleTransitionActor::Execute(CurrentTransition);
    } else {
        co_await WhenAll(UE5Coro::Latent::UntilDelegate(Battlefield->OnLevelShown),
                         ABattleTransitionActor::Execute(CurrentTransition));
    }

    check(RegisteredBattle.IsValid())
    co_await RegisteredBattle->Initialize(std::move(Info));

    auto Result =
        co_await RegisteredBattle->ConductBattle(GetWorld()->GetGameInstance()->GetPrimaryPlayerController(false));
    co_await ExitBattle(Battlefield, StreamingStates);
    co_return Result;
}

UE5Coro::TCoroutine<> UBattleTransitionSubsystem::ExitBattle(ULevelStreamingDynamic *Battlefield,
                                                             const TArray<FLevelStreamingVolumeState> &StreamingStates,
                                                             FForceLatentCoroutine) const {
    check(Battlefield != nullptr)
    FLatentActionInfo LatentActionInfo;
    UGameplayStatics::UnloadStreamLevelBySoftObjectPtr(this, Battlefield->GetWorldAsset(), LatentActionInfo, false);

    for (const auto &[Volume, bDisabled] : StreamingStates) {
        Volume->bDisabled = bDisabled;
    }

    auto PlayerController = GetWorld()->GetGameInstance()->GetPrimaryPlayerController(false);
    PlayerController->GetPawn()->EnableInput(PlayerController);

    co_await Battlefield->OnLevelUnloaded;
    Battlefield = nullptr;
}