// "Unreal Pokémon" created by Retro & Chill.

#include "Settings/DependencyInjectionSettings.h"

#include <functional>

UDependencyInjectionSettings::UDependencyInjectionSettings(const FObjectInitializer &ObjectInitializer) : UDeveloperSettings(ObjectInitializer) {
    FInjectionRegistry::GetInstance()
        .ForEachInjection(std::bind_front(&UDependencyInjectionSettings::AddInjection, this));
}

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

TSubclassOf<UObject> UDependencyInjectionSettings::GetObtainedBlockClass() const {
    return ObtainedBlockClass;
}

TSubclassOf<UObject> UDependencyInjectionSettings::GetBagClass() const {
    return BagClass;
}

void UDependencyInjectionSettings::AddInjection(const IRegisteredInjection &Injection) {
    if (TargetInjections.Contains(Injection.GetInterfaceClass()))
        return;
        
    TargetInjections.Add(Injection.GetInterfaceClass(), Injection.GetFirstInjectableClass());
}