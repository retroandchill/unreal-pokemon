// "Unreal Pokémon" created by Retro & Chill.

#include "Nodes/GainExpOnFaint.h"
#include "Battle/Battle.h"
#include "Battle/Battlers/Battler.h"
#include "Battle/Battlers/BattlerHelpers.h"

UGainExpOnFaint *UGainExpOnFaint::GainExpOnFaint(const UObject *WorldContextObject,
                                                 const TArray<TScriptInterface<IBattler>> &Battlers) {
    auto Node = NewObject<UGainExpOnFaint>();
    Node->SetWorldContext(WorldContextObject);
    Node->Battlers = Battlers;
    return Node;
}

UE5Coro::TCoroutine<> UGainExpOnFaint::ExecuteCoroutine(FForceLatentCoroutine) {
    co_await UBattlerHelpers::GainExpOnFaint(Battlers);
    OnComplete.Broadcast();
}