// "Unreal Pokémon" created by Retro & Chill.


#include "Battle/Transitions/BattleTransitionSubsystem.h"
#include "Battle/Battle.h"
#include "Engine/LevelStreamingDynamic.h"
#include "Kismet/GameplayStatics.h"
#include "Settings/BaseSettings.h"

void UBattleTransitionSubsystem::Initialize(FSubsystemCollectionBase &Collection) {
    Super::Initialize(Collection);
    BattleMap = Pokemon::FBaseSettings::Get().GetDefaultBattleScene();
}

void UBattleTransitionSubsystem::SetBattleMap(const TSoftObjectPtr<UWorld> &NewBattleMap) {
    BattleMap = NewBattleMap;
}

void UBattleTransitionSubsystem::SetRegisteredBattle(const TScriptInterface<IBattle> &Battle) {
    RegisteredBattle = Battle;
    Battle->BindToOnBattleEnd(FOnBattleEnd::FDelegate::CreateUObject(this, &UBattleTransitionSubsystem::ExitBattle));
}

void UBattleTransitionSubsystem::InitiateBattle(const FBattleInfo& Info) {
    static auto &BattleLevelOffset = Pokemon::FBaseSettings::Get().GetBattleSceneOffset();
    bool bSuccess;
    Battlefield = ULevelStreamingDynamic::LoadLevelInstanceBySoftObjectPtr(this, BattleMap, BattleLevelOffset, FRotator(), bSuccess);
    check(bSuccess)
    BattleInfo.Emplace(Info);
    Battlefield->OnLevelShown.AddDynamic(this, &UBattleTransitionSubsystem::SetUpBattle);
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
    RegisteredBattle->Initialize(BattleInfo.GetValue());
}

void UBattleTransitionSubsystem::ExitBattle(EBattleResult Result) {
    check(Battlefield != nullptr)
    FLatentActionInfo LatentActionInfo;
    UGameplayStatics::UnloadStreamLevelBySoftObjectPtr(this, Battlefield->GetWorldAsset(),
                                                           LatentActionInfo, false);
    check(BattleInfo.IsSet())
    if (Result == EBattleResult::Victory || Result == EBattleResult::Inconclusive || BattleInfo->bLossAllowed) {
        BattleFinished.Broadcast(Result);
    } else {
        // If the player loses we want all script callbacks to be removed
        BattleFinished.Clear();
    }
    
    Battlefield = nullptr;
    BattleInfo.Reset();
}