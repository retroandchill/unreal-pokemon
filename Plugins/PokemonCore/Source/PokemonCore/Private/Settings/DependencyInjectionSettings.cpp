// "Unreal Pokémon" created by Retro & Chill.


#include "Settings/DependencyInjectionSettings.h"

TSubclassOf<UObject> UDependencyInjectionSettings::GetPokemonClass() const {
    return PokemonClass;
}

TSubclassOf<UObject> UDependencyInjectionSettings::GetStatBlockClass() const {
    return StatBlockClass;
}

TSubclassOf<UObject> UDependencyInjectionSettings::GetMoveBlockClass() const {
    return MoveBlockClass;
}

TSubclassOf<UObject> UDependencyInjectionSettings::GetAbilityBlockClass() const {
    return AbilityBlockClass;
}