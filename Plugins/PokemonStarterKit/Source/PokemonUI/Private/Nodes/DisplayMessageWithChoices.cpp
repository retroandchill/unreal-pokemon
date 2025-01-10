// "Unreal Pokémon" created by Retro & Chill.
#include "Nodes/DisplayMessageWithChoices.h"
#include "PokemonUISettings.h"
#include "PrimaryGameLayout.h"
#include "RetroLib/Functional/Delegates.h"
#include "Screens/TextDisplayScreen.h"
#include "Utilities/PokemonUIAsyncActions.h"

class UPokemonUISettings;

UDisplayMessageWithChoices *UDisplayMessageWithChoices::DisplayMessageWithChoices(const UObject *WorldContextObject,
                                                                                  FText Message,
                                                                                  const TArray<FText> &Choices) {
    auto Node = NewObject<UDisplayMessageWithChoices>();
    Node->SetWorldContext(WorldContextObject);
    Node->Message = std::move(Message);
    Node->Choices = Choices;
    return Node;
}

UE5Coro::TCoroutine<> UDisplayMessageWithChoices::ExecuteCoroutine(FForceLatentCoroutine Coro) {
    auto [ChoiceIndex, ChoiceID] = co_await Pokemon::UI::DisplayMessageWithChoices(GetWorldContext(), Message, Choices);
    OnChoiceSelected.Broadcast(ChoiceIndex, ChoiceID);
}