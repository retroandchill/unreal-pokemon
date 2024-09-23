// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "DependencyInjectionSubsystem.h"
#include "Kismet/GameplayStatics.h"

namespace UnrealInjector {

    /**
     * Lookup the first injectable object type for the given type
     * @tparam T The raw interface type.
     * @return The first class that can be used
     */
    template <typename T>
    UClass *GetFirstInjectableObject() {
        return GetFirstInjectableObject(T::UClassType::StaticClass());
    }

    /**
     * Lookup the first injectable object type for the given type
     * @param InterfaceType The UInterface type.
     * @return The first class that can be used
     */
    UNREALINJECTOR_API UClass *GetFirstInjectableObject(const TSubclassOf<UInterface> &InterfaceType);

    /**
     * Call to the Dependency Injection subsystem to create a new injected dependency.
     * @tparam T The type of the dependency being injected
     * @tparam A The arguments to the Initialize method on the interface
     * @param Outer The object that will serve as the outer when creating the object
     * @param Args The arguments to forward to the initialize method
     * @return The created interface
     */
    template <typename T, typename... A>
    TScriptInterface<T> NewInjectedDependency(UObject *Outer, A &&...Args) {
        auto Subsystem = UGameplayStatics::GetGameInstance(Outer)->GetSubsystem<UDependencyInjectionSubsystem>();
        check(Subsystem != nullptr)
        return Subsystem->InjectDependency<T>(Outer, Forward<A>(Args)...);
    }

} // namespace UnrealInjector