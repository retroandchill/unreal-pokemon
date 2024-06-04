// "Unreal Pokémon" created by Retro & Chill.


#include "Handlers/MenuHandler.h"

FName UMenuHandler::GetID() const {
    return ID;
}

const FText &UMenuHandler::GetText() const {
    return Text;
}