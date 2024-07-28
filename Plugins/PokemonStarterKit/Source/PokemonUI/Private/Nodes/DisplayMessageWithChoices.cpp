// "Unreal Pokémon" created by Retro & Chill.
#include "Nodes/DisplayMessageWithChoices.h"
#include "PokemonUISettings.h"
#include "PrimaryGameLayout.h"
#include "Screens/TextDisplayScreen.h"

class UPokemonUISettings;

UDisplayMessageWithChoices *UDisplayMessageWithChoices::DisplayMessageWithChoices(const UObject *WorldContextObject,
                                                                                  FText Message,
                                                                                  const TArray<FText> &Choices) {
    auto Node = NewObject<UDisplayMessageWithChoices>();
    Node->WorldContextObject = WorldContextObject;
    Node->Message = MoveTemp(Message);
    Node->Choices = Choices;
    return Node;
}

void UDisplayMessageWithChoices::Activate() {
    auto Layout = UPrimaryGameLayout::GetPrimaryGameLayoutForPrimaryPlayer(WorldContextObject);
    auto Screen = Cast<UTextDisplayScreen>(Layout->GetLayerWidget(RPG::Menus::OverlayMenuLayerTag)->GetActiveWidget());
    if (Screen == nullptr) {
        auto ScreenClass = GetDefault<UPokemonUISettings>()->TextScreenClass.TryLoadClass<UTextDisplayScreen>();
        Screen = Layout->PushWidgetToLayerStack<UTextDisplayScreen>(RPG::Menus::OverlayMenuLayerTag, ScreenClass);
    }
    Screen->DisplayChoices(Message, Choices);
    Screen->ProcessChoice.AddUniqueDynamic(this, &UDisplayMessageWithChoices::ExecuteOnChoiceSelected);
}

void UDisplayMessageWithChoices::ExecuteOnChoiceSelected(int32 ChoiceIndex, FName ChoiceID) {
    OnChoiceSelected.Broadcast(ChoiceIndex, ChoiceID);
    OnChoiceSelected.Clear();
    auto Layout = UPrimaryGameLayout::GetPrimaryGameLayoutForPrimaryPlayer(WorldContextObject);
    auto Screen = Cast<UTextDisplayScreen>(Layout->GetLayerWidget(RPG::Menus::OverlayMenuLayerTag)->GetActiveWidget());
    if (Screen == nullptr)
        return;

    Screen->ProcessChoice.RemoveDynamic(this, &UDisplayMessageWithChoices::ExecuteOnChoiceSelected);
    SetReadyToDestroy();
}
