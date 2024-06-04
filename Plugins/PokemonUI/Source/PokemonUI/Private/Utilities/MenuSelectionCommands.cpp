// "Unreal Pokémon" created by Retro & Chill.


#include "Utilities/MenuSelectionCommands.h"

#include <functional>

void UMenuSelectionCommands::SelectPokemonFromParty(const UObject* WorldContext, const FText &HelpText, const FPokemonSelectedDynamic &Action) {
    //SelectPokemonFromParty(WorldContext, HelpText, FOnPokemonSelected::CreateLambda(std::bind_front(&FPokemonSelectedDynamic::ExecuteIfBound, &Action)));
}

void UMenuSelectionCommands::SelectPokemonFromParty(const UObject *WorldContext, const FText &HelpText,
    const FOnPokemonSelected &Action) {
    
}

void UMenuSelectionCommands::SelectItemFromBag(const FItemSelectedDynamic &Action) {
}