// "Unreal Pokémon" created by Retro & Chill.

#include "Battle/Events/Moves/CriticalHitRateCalculationPayload.h"
#include "Battle/Moves/MoveEvaluationHelpers.h"

FCriticalHitRateCalculationData::FCriticalHitRateCalculationData(const TScriptInterface<IBattleMove> &Move,
                                                                 const TScriptInterface<IBattler> &User,
                                                                 const TScriptInterface<IBattler> &Target,
                                                                 ECriticalOverride Override,
                                                                 int32 CriticalHitRateStages)
    : Move(Move), User(User), Target(Target), Override(Override), CriticalHitRateStages(CriticalHitRateStages)
{
}

UCriticalHitRateCalculationPayload *UCriticalHitRateCalculationPayload::Create(
    const TScriptInterface<IBattleMove> &Move, const TScriptInterface<IBattler> &User,
    const TScriptInterface<IBattler> &Target, ECriticalOverride Override, int32 CriticalHitRateStages)
{
    auto Ret = NewObject<UCriticalHitRateCalculationPayload>();
    Ret->Data = MakeShared<FCriticalHitRateCalculationData>(Move, User, Target, Override, CriticalHitRateStages);
    return Ret;
}

const FCriticalHitRateCalculationData &UCriticalHitRateCalculationPayload::GetData() const
{
    check(Data != nullptr)
    return *Data;
}

const TScriptInterface<IBattler> &UCriticalHitRateCalculationPayload::GetUser() const
{
    check(Data != nullptr)
    return Data->User;
}

const TScriptInterface<IBattler> &UCriticalHitRateCalculationPayload::GetTarget() const
{
    check(Data != nullptr)
    return Data->Target;
}

void UCriticalHitRateCalculationPayload::SetCriticalHitRateOverride(ECriticalOverride Override) const
{
    check(Data != nullptr)
    Data->Override = UMoveEvaluationHelpers::ApplyCriticalHitOverride(Data->Override, Override);
}

void UCriticalHitRateCalculationPayload::SetCriticalHitRateStages(int32 Stages) const
{
    check(Data != nullptr)
    Data->CriticalHitRateStages = Stages;
}