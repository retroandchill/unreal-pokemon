// "Unreal Pokémon" created by Retro & Chill.


#include "Battle/Transitions/BattleTransitionSubsystem.h"
#include "Battle/Battle.h"
#include "Engine/LevelStreamingDynamic.h"
#include "Kismet/GameplayStatics.h"

void UBattleTransitionSubsystem::SetRegisteredBattle(const TScriptInterface<IBattle> &Battle) {
    RegisteredBattle = Battle;
    Battle->BindToOnBattleEnd(FOnBattleEnd::FDelegate::CreateUObject(this, &UBattleTransitionSubsystem::ExitBattle));
}

void UBattleTransitionSubsystem::InitiateWildBattle(const FBattleInfo& Info, const TSoftObjectPtr<UWorld>& BattleMap) {
    static FVector BattleLevelOffset(0, 0, -5000);
    bool bSuccess;
    Battlefield = ULevelStreamingDynamic::LoadLevelInstanceBySoftObjectPtr(this, BattleMap, BattleLevelOffset, FRotator(), bSuccess);
    check(bSuccess)
    BattleInfo.Emplace(Info);
    Battlefield->OnLevelShown.AddDynamic(this, &UBattleTransitionSubsystem::SetUpBattle);
}

void UBattleTransitionSubsystem::SetUpBattle() {
    check(RegisteredBattle.IsValid())
    check(BattleInfo.IsSet())
    RegisteredBattle->Initialize(BattleInfo.GetValue());
    BattleInfo.Reset();
}

void UBattleTransitionSubsystem::ExitBattle() {
    check(Battlefield != nullptr)
    FLatentActionInfo LatentActionInfo;
    UGameplayStatics::UnloadStreamLevelBySoftObjectPtr(this, Battlefield->GetWorldAsset(),
                                                           LatentActionInfo, false);
    Battlefield = nullptr;
}