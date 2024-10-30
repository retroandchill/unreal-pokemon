// "Unreal Pokémon" created by Retro & Chill.

#include "Globals/PropertyRegistration.h"

using namespace Blueprint::Globals;

FPropertyTraitRegistry &FPropertyTraitRegistry::Get() {
    static FPropertyTraitRegistry Instance;
    return Instance;
}