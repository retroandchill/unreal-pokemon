// "Unreal Pokémon" created by Retro & Chill.


#include "Battle/Moves/HitCheckPayload.h"

FHitCheckData::FHitCheckData(const TScriptInterface<IBattleMove> &Move, const TScriptInterface<IBattler> &User,
    const TScriptInterface<IBattler> &Target, int32 BaseAccuracy) : Move(Move), User(User), Target(Target), BaseAccuracy(BaseAccuracy) {
}

UHitCheckPayload * UHitCheckPayload::Create(const TScriptInterface<IBattleMove> &Move,
    const TScriptInterface<IBattler> &User, const TScriptInterface<IBattler> &Target, int32 BaseAccuracy) {
    auto Ret = NewObject<UHitCheckPayload>();
    Ret->Data = MakeShared<FHitCheckData>(Move, User, Target, BaseAccuracy);
    return Ret;
}

const FHitCheckData & UHitCheckPayload::GetData() const {
    check(Data != nullptr)
    return *Data;
}

void UHitCheckPayload::SetBaseAccuracy(int32 Amount) const {
    check(Data != nullptr)
    Data->BaseAccuracy = Amount;
}