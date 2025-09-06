// "Unreal Pokémon" created by Retro & Chill.

#include "Nodes/SelectPokemonFromParty.h"
#include "RetroLib/Optionals/IfPresentOrElse.h"
#include "Utilities/PokemonCoroutineDispatcher.h"

USelectPokemonFromParty *USelectPokemonFromParty::SelectPokemonFromParty(const UObject *WorldContextObject,
                                                                         FText HelpText)
{
    auto Node = NewObject<USelectPokemonFromParty>();
    Node->SetWorldContext(WorldContextObject);
    Node->HelpText = HelpText;
    return Node;
}

UE5Coro::TCoroutine<> USelectPokemonFromParty::ExecuteCoroutine(FForceLatentCoroutine)
{
    Retro::Optionals::IfPresentOrElse(
        co_await IPokemonCoroutineDispatcher::Get(GetWorldContext()).SelectPokemonFromParty(),
        [&](const FSelectedPokemonHandle &Handle) {
            OnSelected.Broadcast(Handle.GetScreen(), Handle.GetTrainer(), Handle.GetIndex());
        },
        [this] { OnCanceled.Broadcast(); });
}