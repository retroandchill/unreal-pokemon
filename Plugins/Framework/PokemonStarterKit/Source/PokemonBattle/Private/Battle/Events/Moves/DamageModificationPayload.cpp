// "Unreal Pokémon" created by Retro & Chill.

#include "Battle//Events/Moves/DamageModificationPayload.h"

FDamageCalculationData::FDamageCalculationData(const TScriptInterface<IBattler> &User,
                                               const TScriptInterface<IBattler> &Target, int32 TargetCount, FName Type,
                                               int32 BasePower)
    : User(User), Target(Target), TargetCount(TargetCount), Type(Type), BasePower(BasePower) {
}

UDamageModificationPayload *UDamageModificationPayload::Create(const TScriptInterface<IBattler> &User,
                                                               const TScriptInterface<IBattler> &Target,
                                                               int32 TargetCount, FName Type, int32 BasePower) {
    auto Ret = NewObject<UDamageModificationPayload>();
    Ret->Data = MakeShared<FDamageCalculationData>(User, Target, TargetCount, Type, BasePower);
    return Ret;
}

const FDamageCalculationData &UDamageModificationPayload::GetData() const {
    check(Data != nullptr)
    return *Data;
}

const TScriptInterface<IBattler> &UDamageModificationPayload::GetUser() const {
    check(Data != nullptr)
    return Data->User;
}

const TScriptInterface<IBattler> &UDamageModificationPayload::GetTarget() const {
    check(Data != nullptr)
    return Data->Target;
}

void UDamageModificationPayload::SetPowerMultiplier(float Amount) const {
    check(Data != nullptr)
    Data->PowerMultiplier = Amount;
}

void UDamageModificationPayload::SetAttackMultiplier(float Amount) const {
    check(Data != nullptr)
    Data->AttackMultiplier = Amount;
}

void UDamageModificationPayload::SeDefenseMultiplier(float Amount) const {
    check(Data != nullptr)
    Data->DefenseMultiplier = Amount;
}

void UDamageModificationPayload::SetFinalDamageMultiplier(float Amount) const {
    check(Data != nullptr)
    Data->FinalDamageMultiplier = Amount;
}