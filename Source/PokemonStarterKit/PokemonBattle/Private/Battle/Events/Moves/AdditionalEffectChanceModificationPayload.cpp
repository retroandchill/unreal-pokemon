// "Unreal Pokémon" created by Retro & Chill.

#include "Battle/Events/Moves/AdditionalEffectChanceModificationPayload.h"

FAdditionalEffectChanceModificationData::FAdditionalEffectChanceModificationData(
    const TScriptInterface<IBattler> &User, const TScriptInterface<IBattler> &Target, int32 AdditionalEffectChance)
    : User(User), Target(Target), AdditionalEffectChance(static_cast<float>(AdditionalEffectChance))
{
}

UAdditionalEffectChanceModificationPayload *UAdditionalEffectChanceModificationPayload::Create(
    const TScriptInterface<IBattler> &User, const TScriptInterface<IBattler> &Target, int32 AdditionalEffectChance)
{
    auto Payload = NewObject<UAdditionalEffectChanceModificationPayload>();
    Payload->Data = MakeShared<FAdditionalEffectChanceModificationData>(User, Target, AdditionalEffectChance);
    return Payload;
}

const FAdditionalEffectChanceModificationData &UAdditionalEffectChanceModificationPayload::GetData() const
{
    check(Data != nullptr)
    return *Data;
}

void UAdditionalEffectChanceModificationPayload::SetAdditionalEffectChance(float AdditionalEffectChance) const
{
    check(Data != nullptr)
    Data->AdditionalEffectChance = AdditionalEffectChance;
}