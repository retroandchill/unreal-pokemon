// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "DependencyInjectionSubsystem.h"
#include "Kismet/GameplayStatics.h"

namespace UnrealInjector {

/**
 * Lookup the first injectable object type for the given type
 * @tparam T The raw interface type.
 * @return The 
 */
template <typename T>
UClass* GetFirstInjectableObject() {
    return GetFirstInjectableObject(T::UClassType::StaticClass());
}

/**
 * Lookup the first injectable object type for the given type
 * @param InterfaceType The UInterface type.
 * @return The 
 */
UNREALINJECTOR_API UClass* GetFirstInjectableObject(TSubclassOf<UInterface> InterfaceType);

/**
 * Call to the Dependency Injection subsystem to create a new injected dependency.
 * @tparam T The type of the dependency being injected
 * @tparam A The arguments to the Initialize method on the interface
 * @param WorldContext The object used to get the game instance
 * @param Args The arguments to forward to the initialize method
 * @return The created interface
 */
template <typename T, typename... A>
TScriptInterface<T> NewInjectedDependency(const UObject* WorldContext, A&&... Args) {
    auto Subsystem = UGameplayStatics::GetGameInstance(WorldContext)->GetSubsystem<UDependencyInjectionSubsystem>();
    check(Subsystem != nullptr);
    return Subsystem->InjectDependency<T>(Forward<A>(Args)...);
}

}