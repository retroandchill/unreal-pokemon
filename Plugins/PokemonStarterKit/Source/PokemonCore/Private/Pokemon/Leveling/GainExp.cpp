// "Unreal Pokémon" created by Retro & Chill.

#include "Pokemon/Leveling/GainExp.h"
#include "Pokemon/Pokemon.h"
#include "Pokemon/Stats/StatBlock.h"

UGainExp *UGainExp::GainExp(const TScriptInterface<IPokemon> &Pokemon, int32 ExpAmount, bool bShowMessages) {
    auto Node = NewObject<UGainExp>();
    Node->Pokemon = Pokemon;
    Node->ExpAmount = ExpAmount;
    Node->bShowMessages = bShowMessages;
    return Node;
}

void UGainExp::Activate() {
    FLevelUpEnd LevelUpEnd;
    LevelUpEnd.BindDynamic(this, &UGainExp::ExecuteAfterExpGain);
    Pokemon->GetStatBlock()->GainExp(ExpAmount, bShowMessages, LevelUpEnd);
}

void UGainExp::ExecuteAfterExpGain() {
    AfterExpGain.Broadcast();
    SetReadyToDestroy();
}