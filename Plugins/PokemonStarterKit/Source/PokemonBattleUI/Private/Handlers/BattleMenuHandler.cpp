// "Unreal Pokémon" created by Retro & Chill.

#include "Handlers/BattleMenuHandler.h"

const FText &UBattleMenuHandler::GetOptionText() const {
    return OptionText;
}

const FSlateBrush &UBattleMenuHandler::GetBackgroundImage() const {
    return BackgroundImage;
}

void UBattleMenuHandler::OnSelected_Implementation(UPokemonBattleScreen *BattleScreen) {
    // No native implementation here, but we need it for subclasses
}