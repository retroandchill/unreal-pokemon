﻿// "Unreal Pokémon" created by Retro & Chill.

#include "Battle/Actions/BattleActionBase.h"

FBattleActionBase::FBattleActionBase(const TScriptInterface<IBattler> &BattlerIn) : Battler(BattlerIn) {
}

void FBattleActionBase::AddReferencedObjects(FReferenceCollector &Collector) {
    Collector.AddReferencedObject(Battler.GetObjectRef());
}

const TScriptInterface<IBattler> &FBattleActionBase::GetBattler() const {
    return Battler;
}

void FBattleActionBase::Execute() {
    Executing = true;
}

bool FBattleActionBase::IsExecuting() const {
    return Executing;
}

void FBattleActionBase::BindToActionFinished(FOnActionFinished &&Delegate) {
    OnActionFinished = MoveTemp(Delegate);
}

FOnActionFinished & FBattleActionBase::GetOnActionFinished() {
    return OnActionFinished;
}