// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Ranges/RangeConcepts.h"
#include "Ranges/Concepts/Pointers.h"
#include "Ranges/Pointers/ValidPtr.h"
#include "Ranges/Utilities/Unreachable.h"

PRAGMA_DISABLE_UNREACHABLE_CODE_WARNINGS

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
        
        /**
         * Checks if the given value is an instance of the specified type T.
         *
         * @param Value The value to be checked. This can be a subclass of UObject, an interface, or a class that implements UnrealInterface.
         * @return True if the value is an instance of type T, otherwise false.
         */
        template <typename U>
            requires std::derived_from<U, UObject> || UnrealInterface<U>
        constexpr bool operator()(const U& Value) const requires std::derived_from<T, UObject> {
            if constexpr (std::derived_from<U, T>) {
                // Trivial case, U is a subclass of T, thus we can always assume this is true
                return true;
            } else if constexpr (std::derived_from<U, UObject>) {
                return Value.template IsA<T>();
            } else if (UnrealInterface<U>) {
                return Value.template Implements<typename U::UClassType>();
            }

            Unreachable();
        }
        
        /**
         * Checks if the given value is an instance of the specified Unreal Interface type T.
         *
         * @param Value The value to be checked. This value can be a subclass of UObject, implement Unreal Engine interfaces,
         * or be a class derived from T.
         * @return True if the value is an instance of type T, otherwise false.
         */
        template <typename U>
            requires std::derived_from<U, UObject> || UnrealInterface<U>
        constexpr bool operator()(const U& Value) const requires UnrealInterface<T> {
            if constexpr (std::derived_from<U, T>) {
                // Trivial case, U is a subclass of T, thus we can always assume this is true
                return true;
            } else if constexpr (std::derived_from<U, UObject>) {
                return Value.template Implements<typename T::UClassType>();
            } else if (UnrealInterface<U>) {
                return Value._getUObject().template Implements<typename T::UClassType>();
            }

            Unreachable();
        }
        
        /**
         * Checks if the given value is an instance of the specified field type T.
         *
         * @param Value The value to be checked. The value must be a type derived from FField.
         * @return True if the value is an instance of the specified type T, otherwise false.
         */
        template <typename U>
            requires std::derived_from<U, FField>
        constexpr bool operator()(const U& Value) const requires std::derived_from<T, FField> {
            if constexpr (std::derived_from<U, T>) {
                // Trivial case, U is a subclass of T, thus we can always assume this is true
                return true;
            } else if constexpr (std::derived_from<U, FField>) {
                return Value.template IsA<T>();
            }

            Unreachable();
        }

        /**
         * Checks if the given pointer is valid and if the dereferenced value is an instance of the expected type.
         *
         * @param Ptr The pointer to be checked, which should dereference to a valid type.
         * @return True if the pointer is valid and the dereferenced value is an instance of the expected type, otherwise false.
         */
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

PRAGMA_RESTORE_UNREACHABLE_CODE_WARNINGS