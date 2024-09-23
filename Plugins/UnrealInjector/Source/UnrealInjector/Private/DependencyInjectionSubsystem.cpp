// "Unreal Pokémon" created by Retro & Chill.

#include "DependencyInjectionSubsystem.h"
#include "DependencyInjectionSettings.h"

void UDependencyInjectionSubsystem::Initialize(FSubsystemCollectionBase &Collection) {
    Super::Initialize(Collection);

    for (auto &[Interface, Implementation] : GetDefault<UDependencyInjectionSettings>()->TargetInjections) {
        TargetInjections.Emplace(Interface.TryLoadClass<UInterface>(), Implementation.TryLoadClass<UObject>());
    }
}