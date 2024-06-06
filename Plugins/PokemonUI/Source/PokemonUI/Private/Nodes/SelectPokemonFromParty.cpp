// "Unreal Pokémon" created by Retro & Chill.

#include "Nodes/SelectPokemonFromParty.h"
#include "RPGMenusSubsystem.h"
#include "Screens/PokemonSelectScreen.h"

USelectPokemonFromParty *
USelectPokemonFromParty::SelectPokemonFromParty(const UObject *WorldContextObject, FText HelpText,
                                                TSubclassOf<UPokemonSelectScreen> ScreenClass) {
    auto Node = NewObject<USelectPokemonFromParty>();
    Node->WorldContextObject = WorldContextObject;
    Node->ScreenClass = ScreenClass;
    Node->HelpText = HelpText;
    return Node;
}

void USelectPokemonFromParty::Activate() {
    auto Controller = WorldContextObject->GetWorld()->GetFirstPlayerController();
    auto Screen = Controller->GetLocalPlayer()->GetSubsystem<URPGMenusSubsystem>()->AddScreenToStack(ScreenClass);
    Screen->GetOnPokemonSelect().BindUObject(this, &USelectPokemonFromParty::ExecuteOnSelected);
    Screen->GetOnScreenClosed().AddDynamic(this, &USelectPokemonFromParty::ExecuteOnCanceled);
    Screen->SetHelpText(HelpText);
}

void USelectPokemonFromParty::ExecuteOnSelected(const TScriptInterface<IPartyScreen> &Screen,
                                                const TScriptInterface<ITrainer> &Trainer, int32 Index) {
    OnSelected.Broadcast(Screen, Trainer, Index);
}

void USelectPokemonFromParty::ExecuteOnCanceled() {
    OnCanceled.Broadcast();
}