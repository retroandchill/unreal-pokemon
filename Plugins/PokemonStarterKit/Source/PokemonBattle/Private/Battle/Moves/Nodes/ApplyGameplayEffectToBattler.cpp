// "Unreal Pokémon" created by Retro & Chill.


#include "Battle/Moves/Nodes/ApplyGameplayEffectToBattler.h"
#include "Battle/Moves/BattleMoveFunctionCode.h"

UApplyGameplayEffectToBattler * UApplyGameplayEffectToBattler::ApplyGameplayEffectToBattler(
    UBattleMoveFunctionCode *Move, const TScriptInterface<IBattler> &Battler, TSubclassOf<UGameplayEffect> EffectClass,
    int32 Level, int32 Stacks) {
    auto Node = NewObject<UApplyGameplayEffectToBattler>();
    Node->SetWorldContext(Battler.GetObject());
    Node->Move = Move;
    Node->Battler = Battler;
    Node->EffectClass = EffectClass;
    Node->Level = Level;
    Node->Stacks = Stacks;
    return Node;
}

UE5Coro::TCoroutine<> UApplyGameplayEffectToBattler::ExecuteCoroutine(FForceLatentCoroutine ForceLatentCoroutine) {
    OnComplete.Broadcast(co_await Move->ApplyGameplayEffectToBattler(Battler, EffectClass, Level, Stacks));
}