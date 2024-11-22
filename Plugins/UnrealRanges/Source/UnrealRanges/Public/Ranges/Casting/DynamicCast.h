// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Ranges/RangeConcepts.h"
#include "Ranges/Concepts/Pointers.h"
#include "Ranges/Pointers/ValidPtr.h"

namespace UE::Ranges {
    /**
     * A casting utility class designed for Unreal Engine objects and fields.
     * Provides a series of operator() overloads to perform dynamic casts with validation.
     * Casting can be done in either checked or unchecked mode, based on the template parameter `bChecked`.
     *
     * @tparam T The type to cast to. This must derive from either UObject, FField, or satisfy the UnrealInterface concept.
     * @tparam bChecked A boolean indicating whether casts should perform runtime checks.
     */
    template <typename T, bool bChecked = false>
        requires std::derived_from<T, UObject> || UnrealInterface<T> || std::derived_from<T, FField>
    struct TDynamicCast {
        template <typename U>
            requires std::derived_from<U, UObject> || UnrealInterface<U>
        constexpr decltype(auto) operator()(U& Object) const requires std::derived_from<T, UObject> || UnrealInterface<T> {
            if constexpr (std::derived_from<U, T>) {
                // Trivial case, no need to actually cast
                return static_cast<T&>(Object);
            } else if constexpr (bChecked) {
                return *CastChecked<T>(&Object);
            } else {
                return TOptional<T&>(Cast<T>(&Object));
            }
        }

        template <typename U>
            requires std::derived_from<U, FField>
        constexpr decltype(auto) operator()(U& Object) const requires std::derived_from<T, FField> {
            if constexpr (std::derived_from<U, T>) {
                // Trivial case, no need to actually cast
                return static_cast<T&>(Object);
            } else if constexpr (bChecked) {
                return *CastFieldChecked<T>(Object);
            } else {
                return TOptional<T&>(CastField<T>(&Object));
            }
        }

        template <typename U>
            requires DereferencesTo<U, const UObject> || DereferencesToInterface<U> || DereferencesTo<U, const FField>
        constexpr decltype(auto) operator()(U &&Ptr) const {
            if constexpr (bChecked) {
                checkf(ValidPtr(std::forward<U>(Ptr)), TEXT("Null pointer detected!"))
                return &operator()(*Ptr);
            } else {
                if (!ValidPtr(std::forward<U>(Ptr))) {
                    return TOptional<T&>(nullptr);
                }
                return static_cast<TOptional<T&>>(operator()(*Ptr));
            }
        }
    };

    /**
     * A constexpr instance of TDynamicCast designed for Unreal Engine objects and fields.
     * Provides utilities for performing dynamic casts with validation, based on template parameters.
     *
     * @tparam T The type to cast to. This type must derive from either UObject, FField, or satisfy the UnrealInterface concept.
     */
    template <typename T>
    constexpr TDynamicCast<T> DynamicCast;
    
    /**
     * A constexpr instance of TDynamicCast, specialized to enforce runtime checks during casting.
     *
     * Designed for Unreal Engine objects and fields, this variable offers a reliable way to perform dynamic casts
     * with runtime validation, ensuring that the cast is safe and conforms to the expected type.
     *
     * @tparam T The type to cast to. This type must derive from either UObject, FField, or satisfy the UnrealInterface concept.
     */
    template <typename T>
    constexpr TDynamicCast<T, true> DynamicCastChecked;
}
