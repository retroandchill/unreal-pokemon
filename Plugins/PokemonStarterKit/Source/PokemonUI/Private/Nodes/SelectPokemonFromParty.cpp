// "Unreal Pokémon" created by Retro & Chill.

#include "Nodes/SelectPokemonFromParty.h"
#include "RetroLib/Optionals/IfPresentOrElse.h"
#include "Utilities/PokemonUIAsyncActions.h"

USelectPokemonFromParty *USelectPokemonFromParty::SelectPokemonFromParty(const UObject *WorldContextObject,
                                                                         FText HelpText) {
    auto Node = NewObject<USelectPokemonFromParty>();
    Node->SetWorldContext(WorldContextObject);
    Node->HelpText = HelpText;
    return Node;
}

UE5Coro::TCoroutine<> USelectPokemonFromParty::ExecuteCoroutine(FForceLatentCoroutine) {
    Retro::Optionals::IfPresentOrElse(
        co_await Pokemon::UI::SelectPokemonFromParty(GetWorldContext(), {}),
        [&](const FSelectedPokemonHandle &Handle) {
            OnSelected.Broadcast(Handle.GetScreen(), Handle.GetTrainer(), Handle.GetIndex());
        },
        [this] { OnCanceled.Broadcast(); });
}