// "Unreal Pokémon" created by Retro & Chill.

#include "Handlers/MenuHandler.h"
#include "Data/Command.h"

FName UMenuHandler::GetID() const {
    return ID;
}

const FText &UMenuHandler::GetText() const {
    return Text;
}

UCommand *UMenuHandler::CreateCommand() {
    return UCommand::CreateBasicCommand(ID, Text, this);
}