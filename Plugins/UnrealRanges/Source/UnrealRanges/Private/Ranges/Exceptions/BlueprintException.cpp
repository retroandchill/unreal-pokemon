// "Unreal Pokémon" created by Retro & Chill.

#include "Ranges/Exceptions/BlueprintException.h"

const char *UE::Ranges::FBlueprintException::what() const {
    return NativeMessage.c_str();
}