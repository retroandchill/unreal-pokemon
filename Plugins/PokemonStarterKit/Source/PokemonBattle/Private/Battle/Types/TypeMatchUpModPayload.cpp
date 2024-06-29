// "Unreal Pokémon" created by Retro & Chill.


#include "Battle/Types/TypeMatchUpModPayload.h"

FTypeMatchUpModData::FTypeMatchUpModData(const TScriptInterface<IBattler> &User, const TScriptInterface<IBattler> &Target, FName AttackingType, float Multiplier) : User(User), Target(Target), AttackingType(AttackingType), Multiplier(Multiplier) {
}

UTypeMatchUpModPayload * UTypeMatchUpModPayload::Create(const TScriptInterface<IBattler> &User, const TScriptInterface<IBattler> &Target, FName AttackingType, float Multiplier) {
    auto Ret = NewObject<UTypeMatchUpModPayload>();
    Ret->Data = MakeShared<FTypeMatchUpModData>(User, Target, AttackingType, Multiplier);
    return Ret;
}

const FTypeMatchUpModData &UTypeMatchUpModPayload::GetData() const {
    check(Data != nullptr)
    return *Data;
}

void UTypeMatchUpModPayload::SetMultiplier(float Multiplier) const {
    check(Data != nullptr)
    Data->Multiplier = Multiplier;
}