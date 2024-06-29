// "Unreal Pokémon" created by Retro & Chill.


#include "Battle/Moves/SuccessCheckAgainstTargetPayload.h"

FTargetSuccessCheckPayload::FTargetSuccessCheckPayload(const TScriptInterface<IBattleMove> &Move,
                                                       const FRunningMessageSet &Messages,
                                                       bool bShowMessages) : Move(Move), Messages(Messages),
                                                                             bShowMessages(bShowMessages) {
}

USuccessCheckAgainstTargetPayload *USuccessCheckAgainstTargetPayload::Create(const TScriptInterface<IBattleMove> &Move,
                                                               const FRunningMessageSet &Messages, bool bShowMessages) {
    auto Ret = NewObject<USuccessCheckAgainstTargetPayload>();
    Ret->Data = MakeShared<FTargetSuccessCheckPayload>(Move, Messages, bShowMessages);
    return Ret;
}

FTargetSuccessCheckPayload &USuccessCheckAgainstTargetPayload::GetData() const {
    check(Data != nullptr)
    return *Data;
}