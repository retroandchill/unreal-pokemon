// "Unreal Pokémon" created by Retro & Chill.


#include "Blueprints/Globals/Registry.h"

UE::Blueprints::FRegistry & UE::Blueprints::FRegistry::Get() {
    static FRegistry Instance;
    return Instance;
}