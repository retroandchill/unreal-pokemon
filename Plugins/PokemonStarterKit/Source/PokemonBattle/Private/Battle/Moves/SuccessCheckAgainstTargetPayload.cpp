// "Unreal Pokémon" created by Retro & Chill.


#include "Battle/Moves/SuccessCheckAgainstTargetPayload.h"

FTargetSuccessCheckPayload::FTargetSuccessCheckPayload(const TScriptInterface<IBattleMove> &Move, const TScriptInterface<IBattler> &User, const TScriptInterface<IBattler> &Target,
                                                       const FRunningMessageSet &Messages,
                                                       bool bShowMessages) : Move(Move), User(User), Target(Target), Messages(Messages),
                                                                             bShowMessages(bShowMessages) {
}

USuccessCheckAgainstTargetPayload *USuccessCheckAgainstTargetPayload::Create(const TScriptInterface<IBattleMove> &Move, const TScriptInterface<IBattler> &User, const TScriptInterface<IBattler> &Target,  
                                                               const FRunningMessageSet &Messages, bool bShowMessages) {
    auto Ret = NewObject<USuccessCheckAgainstTargetPayload>();
    Ret->Data = MakeShared<FTargetSuccessCheckPayload>(Move, User, Target, Messages, bShowMessages);
    return Ret;
}

const FTargetSuccessCheckPayload &USuccessCheckAgainstTargetPayload::GetData() const {
    check(Data != nullptr)
    return *Data;
}

void USuccessCheckAgainstTargetPayload::SetSuccess(bool bSuccess) const {
    check(Data != nullptr)
    Data->bSuccess =bSuccess;
}