// "Unreal Pokémon" created by Retro & Chill.
#include "Nodes/DisplayMessageWithChoices.h"
#include "Screens/TextDisplayScreen.h"
#include "Utilities/PokemonCoroutineDispatcher.h"

class UPokemonUISettings;

UDisplayMessageWithChoices *UDisplayMessageWithChoices::DisplayMessageWithChoices(const UObject *WorldContextObject,
                                                                                  FText Message,
                                                                                  const TArray<FText> &Choices)
{
    auto Node = NewObject<UDisplayMessageWithChoices>();
    Node->SetWorldContext(WorldContextObject);
    Node->Message = std::move(Message);
    Node->Choices = Choices;
    return Node;
}

UE5Coro::TCoroutine<> UDisplayMessageWithChoices::ExecuteCoroutine(FForceLatentCoroutine)
{
    auto &Dispatcher = IPokemonCoroutineDispatcher::Get(GetWorldContext());
    auto [ChoiceIndex, ChoiceID] = co_await Dispatcher.DisplayMessageWithChoices(Message, Choices);
    OnChoiceSelected.Broadcast(ChoiceIndex, ChoiceID);
}