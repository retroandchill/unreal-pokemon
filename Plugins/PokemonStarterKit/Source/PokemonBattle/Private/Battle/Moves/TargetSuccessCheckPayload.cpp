// "Unreal Pokémon" created by Retro & Chill.


#include "Battle/Moves/TargetSuccessCheckPayload.h"

FTargetSuccessCheckPayload::FTargetSuccessCheckPayload(const TScriptInterface<IBattleMove> &Move,
                                                       const FRunningMessageSet &Messages,
                                                       bool bShowMessages) : Move(Move), Messages(Messages),
                                                                             bShowMessages(bShowMessages) {
}

UTargetSuccessCheckPayload *UTargetSuccessCheckPayload::Create(const TScriptInterface<IBattleMove> &Move,
                                                               const FRunningMessageSet &Messages, bool bShowMessages) {
    auto Ret = NewObject<UTargetSuccessCheckPayload>();
    Ret->Data = MakeShared<FTargetSuccessCheckPayload>(Move, Messages, bShowMessages);
    return Ret;
}

FTargetSuccessCheckPayload &UTargetSuccessCheckPayload::GetData() const {
    check(Data != nullptr)
    return *Data;
}