// "Unreal Pokémon" created by Retro & Chill.

#include "Pokemon/Leveling/GainExp.h"
#include "Pokemon/Pokemon.h"
#include "Pokemon/Stats/StatBlock.h"

UGainExp *UGainExp::GainExp(const TScriptInterface<IPokemon> &Pokemon, int32 ExpAmount, bool bShowMessages)
{
    auto Node = NewObject<UGainExp>();
    Node->SetWorldContext(Pokemon.GetObject());
    Node->Pokemon = Pokemon;
    Node->ExpAmount = ExpAmount;
    Node->bShowMessages = bShowMessages;
    return Node;
}

UE5Coro::TCoroutine<> UGainExp::ExecuteCoroutine(FForceLatentCoroutine ForceLatentCoroutine)
{
    co_await Pokemon->GetStatBlock()->GainExp(ExpAmount, bShowMessages);
    AfterExpGain.Broadcast();
}