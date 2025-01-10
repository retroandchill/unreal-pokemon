// "Unreal Pokémon" created by Retro & Chill.

#include "Pokemon/Leveling/ProcessLevelUp.h"
#include "Utilities/PokemonCoroutineDispatcher.h"

UProcessLevelUp *UProcessLevelUp::ProcessLevelUp(const TScriptInterface<IPokemon> &Pokemon,
                                                 const FLevelUpStatChanges &StatChanges, bool bShowMessages) {
    auto Node = NewObject<UProcessLevelUp>();
    Node->SetWorldContext(Pokemon.GetObject());
    Node->Pokemon = Pokemon;
    Node->StatChanges = StatChanges;
    Node->bShowMessages = bShowMessages;
    return Node;
}

UE5Coro::TCoroutine<> UProcessLevelUp::ExecuteCoroutine(FForceLatentCoroutine Coro) {
    auto &Dispatcher = IPokemonCoroutineDispatcher::Get(GetWorldContext());
    if (bShowMessages) {
        co_await Dispatcher.ProcessLevelUp(Pokemon, StatChanges);
    }
    AfterLevelUp.Broadcast();
}