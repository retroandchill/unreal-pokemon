// "Unreal Pokémon" created by Retro & Chill.

#include "Battle/Types/SingleTypeModPayload.h"

FSingleTypeModData::FSingleTypeModData(const TScriptInterface<IBattler> &User, const TScriptInterface<IBattler> &Target,
                                       FName AttackingType, FName DefendingType, float Multiplier)
    : User(User), Target(Target), AttackingType(AttackingType), DefendingType(DefendingType), Multiplier(Multiplier) {
}

USingleTypeModPayload *USingleTypeModPayload::Create(const TScriptInterface<IBattler> &User,
                                                     const TScriptInterface<IBattler> &Target, FName AttackingType,
                                                     FName DefendingType, float Multiplier) {
    auto Ret = NewObject<USingleTypeModPayload>();
    Ret->Data = MakeShared<FSingleTypeModData>(User, Target, AttackingType, DefendingType, Multiplier);
    return Ret;
}

const FSingleTypeModData &USingleTypeModPayload::GetData() const {
    check(Data != nullptr)
    return *Data;
}

void USingleTypeModPayload::SetMultiplier(float Multiplier) const {
    check(Data != nullptr)
    Data->Multiplier = Multiplier;
}