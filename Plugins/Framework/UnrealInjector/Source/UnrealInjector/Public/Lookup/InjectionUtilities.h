// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "InjectableDependency.h"

namespace UnrealInjector {

    /**
     * Lookup the first injectable object type for the given type
     * @tparam T The raw interface type.
     * @return The first class that can be used
     */
    template <typename T>
        requires Retro::UnrealInterface<T>
    TOptional<UClass &> GetFirstInjectableObject() {
        return GetFirstInjectableObject(T::UClassType::StaticClass());
    }

    /**
     * Lookup the first injectable object type for the given type
     * @param Class The class to check injection for
     * @return The first class that can be used
     */
    UNREALINJECTOR_API TOptional<UClass &> GetFirstInjectableObject(const UClass *Class);

    /**
     * Call to the Dependency Injection subsystem to create a new injected dependency.
     * @tparam T The type of the dependency being injected
     * @tparam A The arguments to the Initialize method on the interface
     * @param Outer The object that will serve as the outer when creating the object
     * @param Args The arguments to forward to the initialize method
     * @return The created interface
     */
    template <typename T, typename... A>
        requires Injectable<T> && CanInitialize<T, A...>
    auto NewInjectedDependency(UObject *Outer, A &&...Args) {
        return TInjectionSettings<T>::Get().template Inject<A...>(Outer, std::forward<A>(Args)...);
    }

} // namespace UnrealInjector