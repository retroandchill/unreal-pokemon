// "Unreal Pokémon" created by Retro & Chill.

#include "Battle//Events/Moves/SuccessCheckAgainstTargetPayload.h"

FTargetSuccessCheckPayload::FTargetSuccessCheckPayload(const TScriptInterface<IBattleMove> &Move,
                                                       const TScriptInterface<IBattler> &User,
                                                       const TScriptInterface<IBattler> &Target, bool bShowMessages)
    : Move(Move), User(User), Target(Target), bShowMessages(bShowMessages)
{
}

USuccessCheckAgainstTargetPayload *USuccessCheckAgainstTargetPayload::Create(const TScriptInterface<IBattleMove> &Move,
                                                                             const TScriptInterface<IBattler> &User,
                                                                             const TScriptInterface<IBattler> &Target,
                                                                             bool bShowMessages)
{
    auto Ret = NewObject<USuccessCheckAgainstTargetPayload>();
    Ret->Data = MakeShared<FTargetSuccessCheckPayload>(Move, User, Target, bShowMessages);
    return Ret;
}

const FTargetSuccessCheckPayload &USuccessCheckAgainstTargetPayload::GetData() const
{
    check(Data != nullptr)
    return *Data;
}

const TScriptInterface<IBattler> &USuccessCheckAgainstTargetPayload::GetUser() const
{
    check(Data != nullptr)
    return Data->User;
}

const TScriptInterface<IBattler> &USuccessCheckAgainstTargetPayload::GetTarget() const
{
    check(Data != nullptr)
    return Data->Target;
}

void USuccessCheckAgainstTargetPayload::SetSuccess(bool bSuccess) const
{
    check(Data != nullptr)
    Data->bSuccess = bSuccess;
}