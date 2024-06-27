// "Unreal Pokémon" created by Retro & Chill.


#include "Battle/Moves/MoveAnimation.h"


void AMoveAnimation::Initialize(const TScriptInterface<IBattler> &UserIn,
    const TArray<TScriptInterface<IBattler>> &TargetsIn) {
    User = UserIn;
    Targets = TargetsIn;
}

const TScriptInterface<IBattler> & AMoveAnimation::GetUser() const {
    return User;
}

const TArray<TScriptInterface<IBattler>> & AMoveAnimation::GetTargets() const {
    return Targets;
}

void AMoveAnimation::ExecuteOnMoveAnimationComplete() const {
    check(OnMoveAnimationComplete.IsBound())
    OnMoveAnimationComplete.Execute();
}

FOnMoveAnimationComplete & AMoveAnimation::GetOnMoveAnimationComplete() {
    return OnMoveAnimationComplete;
}