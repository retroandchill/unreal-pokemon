// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Ranges/RangeConcepts.h"
#include "Ranges/Concepts/Pointers.h"
#include "Ranges/Pointers/ValidPtr.h"
#include "Ranges/Utilities/Unreachable.h"

namespace UE::Ranges {
    /**
     * @brief Struct to check if a given value is an instance of a specific Unreal Engine type.
     *
     * This struct provides overloaded call operators to determine if `Value` is an instance of type `T`.
     * The type `T` can be derived from `UObject`, implement Unreal Engine interfaces, or be derived from `FField`.
     *
     * @tparam T The Unreal Engine type to be checked against.
     */
    template <typename T>
        requires std::derived_from<T, UObject> || UnrealInterface<T> || std::derived_from<T, FField>
    struct TInstanceOf {

        template <typename U>
            requires std::derived_from<T, UObject> || UnrealInterface<U>
        constexpr bool operator()(const U& Value) const requires std::derived_from<T, UObject> {
            if constexpr (std::derived_from<U, T>) {
                // Trivial case, U is a subclass of T, thus we can always assume this is true
                return true;
            } else if constexpr (std::derived_from<U, UObject>()) {
                return Value.template IsA<T>();
            } else if (UnrealInterface<U>) {
                return Value.template Implements<typename U::UClassType>();
            }

            Unreachable();
        }

        template <typename U>
            requires std::derived_from<T, UObject> || UnrealInterface<U>
        constexpr bool operator()(const U& Value) const requires UnrealInterface<T> {
            if constexpr (std::derived_from<U, T>) {
                // Trivial case, U is a subclass of T, thus we can always assume this is true
                return true;
            } else if constexpr (std::derived_from<U, UObject>()) {
                return Value._getUObject()->template IsA<T>();
            } else if (UnrealInterface<U>) {
                return Value._getUObject().template Implements<typename U::UClassType>();
            }

            Unreachable();
        }

        template <typename U>
            requires std::derived_from<U, FField>
        constexpr bool operator()(const U& Value) const requires std::derived_from<T, FField> {
            if constexpr (std::derived_from<U, T>) {
                // Trivial case, U is a subclass of T, thus we can always assume this is true
                return true;
            } else if constexpr (std::derived_from<U, UObject>()) {
                return Value.template IsA<T>();
            }

            Unreachable();
        }

        template <typename U>
            requires DereferencesTo<U, const UObject> || DereferencesToInterface<U> || DereferencesTo<U, const FField>
        constexpr bool operator()(U &&Ptr) const {
            return ValidPtr(std::forward<U>(Ptr)) && operator()(*Ptr);
        }
    };

    /**
     * @brief A constant expression for checking if a given value is an instance of a specific Unreal Engine type.
     *
     * `InstanceOf` can be used to determine if a value is an instance of the type `T`. The type `T` can be a class derived from `UObject`,
     * implement Unreal Engine interfaces, or a class derived from `FField`. This helps in ensuring type safety and correctness
     * in various operations involving Unreal Engine types.
     *
     * @tparam T The Unreal Engine type to check against.
     */
    template <typename T>
    constexpr TInstanceOf<T> InstanceOf;
}
