// "Unreal Pokémon" created by Retro & Chill.


#include "Battle/Types/TypeMatchUpModPayload.h"

FTypeMatchUpModData::FTypeMatchUpModData(FName AttackingType, float Multiplier) : AttackingType(AttackingType), Multiplier(Multiplier) {
}

UTypeMatchUpModPayload * UTypeMatchUpModPayload::Create(FName AttackingType, float Multiplier) {
    auto Ret = NewObject<UTypeMatchUpModPayload>();
    Ret->Data = MakeShared<FTypeMatchUpModData>(AttackingType, Multiplier);
    return Ret;
}

FTypeMatchUpModData & UTypeMatchUpModPayload::GetData() const {
    check(Data != nullptr)
    return *Data;
}