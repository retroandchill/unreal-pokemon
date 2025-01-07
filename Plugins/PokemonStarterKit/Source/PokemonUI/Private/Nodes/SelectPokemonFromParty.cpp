// "Unreal Pokémon" created by Retro & Chill.

#include "Nodes/SelectPokemonFromParty.h"
#include "PokemonUISettings.h"
#include "PrimaryGameLayout.h"
#include "RetroLib/Optionals/IfPresentOrElse.h"
#include "Screens/PokemonSelectScreen.h"
#include "Utilities/PokemonUIAsyncActions.h"

USelectPokemonFromParty *USelectPokemonFromParty::SelectPokemonFromParty(const UObject *WorldContextObject,
                                                                         FText HelpText) {
    auto Node = NewObject<USelectPokemonFromParty>();
    Node->WorldContextObject = WorldContextObject;
    Node->HelpText = HelpText;
    return Node;
}

UE5Coro::TCoroutine<> USelectPokemonFromParty::ExecuteCoroutine(FForceLatentCoroutine) {
    Retro::Optionals::IfPresentOrElse(
        co_await Pokemon::UI::SelectPokemonFromParty(WorldContextObject),
        [&](const FSelectedPokemonHandle &Handle) {
            OnSelected.Broadcast(Handle.GetScreen(), Handle.GetTrainer(), Handle.GetIndex());
        },
        [this] { OnCanceled.Broadcast(); });
}