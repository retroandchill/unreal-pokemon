﻿// "Unreal Pokémon" created by Retro & Chill.

#include "Battle/Transitions/InitiateBattle.h"
#include "Battle/Transitions/BattleTransitionSubsystem.h"

UInitiateBattle *UInitiateBattle::InitiateBattle(const UObject *WorldContext, const FBattleInfo &BattleInfo,
                                                 TSubclassOf<ABattleTransitionActor> Transition) {
    auto Node = NewObject<UInitiateBattle>();
    Node->WorldContext = WorldContext;
    Node->BattleInfo = BattleInfo;
    Node->Transition = Transition;
    return Node;
}

void UInitiateBattle::Activate() {
    auto Subsystem = WorldContext->GetWorld()->GetSubsystem<UBattleTransitionSubsystem>();
    check(Subsystem != nullptr)
    OutputExecHandle.Emplace(Subsystem->BindToBattleFinished(
        FBattleFinished::FDelegate::CreateUObject(this, &UInitiateBattle::OnBattleComplete)));
    Subsystem->InitiateBattle(BattleInfo, Transition);
}

void UInitiateBattle::OnBattleComplete(EBattleResult Result) {
    AfterBattle.Broadcast(Result);
    auto Subsystem = WorldContext->GetWorld()->GetSubsystem<UBattleTransitionSubsystem>();
    check(Subsystem != nullptr)
    check(OutputExecHandle.IsSet())
    Subsystem->RemoveFromBattleFinished(*OutputExecHandle);
    OutputExecHandle.Reset();
    SetReadyToDestroy();
}
