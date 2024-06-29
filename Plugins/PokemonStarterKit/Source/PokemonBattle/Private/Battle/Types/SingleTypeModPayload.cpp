// "Unreal Pokémon" created by Retro & Chill.


#include "Battle/Types/SingleTypeModPayload.h"

FSingleTypeModData::FSingleTypeModData(FName AttackingType, FName DefendingType, float Multiplier)
    : AttackingType(AttackingType), DefendingType(DefendingType), Multiplier(Multiplier) {
}

USingleTypeModPayload * USingleTypeModPayload::Create(FName AttackingType, FName DefendingType, float Multiplier) {
    auto Ret = NewObject<USingleTypeModPayload>();
    Ret->Data = MakeShared<FSingleTypeModData>(AttackingType, DefendingType, Multiplier);
    return Ret;
}

FSingleTypeModData & USingleTypeModPayload::GetData() const {
    check(Data != nullptr)
    return *Data;
}