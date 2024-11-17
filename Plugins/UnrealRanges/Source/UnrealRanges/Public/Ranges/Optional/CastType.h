// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Map.h"
#include "OptionalRef.h"
#include "Ranges/Concepts/UObjectPointer.h"

namespace UE::Optionals {

    template <typename T>
    struct TCastType {

        template <typename O, typename S = TContainedOptionalType<O>>
            requires UEOptional<O>
        decltype(auto) operator()(O &&Optional) const {
            if constexpr (Ranges::UObjectPointer<Ranges::TDecayReferenceType<S>> &&
                          (std::derived_from<T, UObject> || Ranges::UnrealInterface<T>)) {
                if constexpr (std::is_lvalue_reference_v<S>) {
                    return Optional | Map([](S Object) { return Cast<T>(&Object); });
                } else {
                    return Optional | Map([](S Object) { return Cast<T>(Object); });
                }
            } else if constexpr (std::derived_from<S, FScriptInterface> &&
                                 (std::derived_from<T, UObject> || Ranges::UnrealInterface<T>)) {
                return Optional | Map([](const S &Object) { return Cast<T>(Object.GetObject()); });
            } else if constexpr (std::is_same_v<S, uint8 *>) {
                return Optional | Map([](S Object) -> T & { return *static_cast<T *>(static_cast<void *>(Object)); });
            } else {
                return Optional | Map([]<typename U>(U &&Object) { return static_cast<T>(std::forward<U>(Object)); });
            }
        }
    };

    /**
     * Cast the given type to another type.
     */
    template <typename T>
    constexpr TOptionalClosure<TCastType<T>> CastType;

} // namespace UE::Optionals
