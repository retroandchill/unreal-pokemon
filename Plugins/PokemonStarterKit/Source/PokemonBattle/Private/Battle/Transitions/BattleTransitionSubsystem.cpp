// "Unreal Pokémon" created by Retro & Chill.

#include "Battle/Transitions/BattleTransitionSubsystem.h"
#include "Battle/Battle.h"
#include "Battle/Transitions/BattleTransitionActor.h"
#include "Engine/LevelStreamingDynamic.h"
#include "Engine/LevelStreamingVolume.h"
#include "Kismet/GameplayStatics.h"
#include "Managers/PokemonSubsystem.h"
#include "PokemonBattleSettings.h"

void UBattleTransitionSubsystem::Initialize(FSubsystemCollectionBase &Collection) {
    Super::Initialize(Collection);
    BattleMap = GetDefault<UPokemonBattleSettings>()->DefaultBattleScene;
}

void UBattleTransitionSubsystem::SetBattleMap(const TSoftObjectPtr<UWorld> &NewBattleMap) {
    BattleMap = NewBattleMap;
}

void UBattleTransitionSubsystem::SetRegisteredBattle(const TScriptInterface<IBattle> &Battle) {
    RegisteredBattle = Battle;
    Battle->BindToOnBattleEnd(FOnBattleEnd::FDelegate::CreateUObject(this, &UBattleTransitionSubsystem::ExitBattle));
}

void UBattleTransitionSubsystem::InitiateBattle(const FBattleInfo &Info,
                                                TSubclassOf<ABattleTransitionActor> Transition) {
    auto PlayerController = GetWorld()->GetGameInstance()->GetPrimaryPlayerController(false);
    PlayerController->GetPawn()->DisableInput(PlayerController);
    static auto &BattleLevelOffset = GetDefault<UPokemonBattleSettings>()->BattleSceneOffset;
    if (Transition != nullptr) {
        using FTransitionBinding = FOnBattleTransitionComplete::FDelegate;

        bBattleInitialized = false;
        CurrentTransition = GetWorld()->SpawnActor<ABattleTransitionActor>(Transition);
        CurrentTransition->BindToOnComplete(FTransitionBinding::CreateWeakLambda(this, [this] {
            if (RegisteredBattle.IsValid() && !bBattleInitialized) {
                RegisteredBattle->Initialize(BattleInfo.GetValue());
                bBattleInitialized = true;
            }

            CurrentTransition = nullptr;
        }));
        CurrentTransition->TransitionToBattle();
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
    Battlefield->OnLevelShown.AddUniqueDynamic(this, &UBattleTransitionSubsystem::SetUpBattle);
}

FDelegateHandle UBattleTransitionSubsystem::BindToBattleFinished(FBattleFinished::FDelegate &&Callback) {
    return BattleFinished.Add(MoveTemp(Callback));
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

void UBattleTransitionSubsystem::ExitBattle(EBattleResult Result) {
    check(Battlefield != nullptr)
    FLatentActionInfo LatentActionInfo;
    UGameplayStatics::UnloadStreamLevelBySoftObjectPtr(this, Battlefield->GetWorldAsset(), LatentActionInfo, false);
    check(BattleInfo.IsSet())
    if (Result != EBattleResult::Defeat || BattleInfo->bLossAllowed) {
        BattleFinished.Broadcast(Result);
    } else {
        // If the player loses we want all script callbacks to be removed
        BattleFinished.Clear();
        UPokemonSubsystem::GetInstance(this).PerformPlayerReset();
    }

    Battlefield = nullptr;
    BattleInfo.Reset();

    for (const auto &[Volume, bDisabled] : StreamingStates) {
        Volume->bDisabled = bDisabled;
    }
    StreamingStates.Reset();
    bBattleInitialized = false;

    auto PlayerController = GetWorld()->GetGameInstance()->GetPrimaryPlayerController(false);
    PlayerController->GetPawn()->EnableInput(PlayerController);
}