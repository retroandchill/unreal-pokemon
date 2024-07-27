// "Unreal Pokémon" created by Retro & Chill.

#include "Nodes/SelectPokemonFromParty.h"
#include "PokemonUISettings.h"
#include "PrimaryGameLayout.h"
#include "Screens/PokemonSelectScreen.h"

USelectPokemonFromParty *USelectPokemonFromParty::SelectPokemonFromParty(const UObject *WorldContextObject, FText HelpText) {
    auto Node = NewObject<USelectPokemonFromParty>();
    Node->WorldContextObject = WorldContextObject;
    Node->HelpText = HelpText;
    return Node;
}

void USelectPokemonFromParty::Activate() {
    auto Layout = UPrimaryGameLayout::GetPrimaryGameLayoutForPrimaryPlayer(WorldContextObject);
    auto ScreenClass = GetDefault<UPokemonUISettings>()->PartyScreenClass.TryLoadClass<UPokemonSelectScreen>();
    auto Screen = Layout->PushWidgetToLayerStack<UPokemonSelectScreen>(RPG::Menus::PrimaryMenuLayerTag, ScreenClass);
    Screen->GetOnPokemonSelect().BindUObject(this, &USelectPokemonFromParty::ExecuteOnSelected);
    Screen->GetOnScreenClosed().AddUniqueDynamic(this, &USelectPokemonFromParty::ExecuteOnCanceled);
}

void USelectPokemonFromParty::ExecuteOnSelected(const TScriptInterface<IPartyScreen> &Screen,
                                                const TScriptInterface<ITrainer> &Trainer, int32 Index) {
    OnSelected.Broadcast(Screen, Trainer, Index);
    SetReadyToDestroy();
}

void USelectPokemonFromParty::ExecuteOnCanceled() {
    OnCanceled.Broadcast();
    SetReadyToDestroy();
}