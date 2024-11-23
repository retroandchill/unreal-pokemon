// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Ranges/Concepts/Pointers.h"
#include "Ranges/Pointers/ValidPtr.h"
#include "Ranges/RangeConcepts.h"

namespace UE::Ranges {
    /**
     * A casting utility class designed for Unreal Engine objects and fields.
     * Provides a series of operator() overloads to perform dynamic casts with validation.
     * Casting can be done in either checked or unchecked mode, based on the template parameter `bChecked`.
     *
     * @tparam T The type to cast to. This must derive from either UObject, FField, or satisfy the UnrealInterface
     * concept.
     * @tparam bChecked A boolean indicating whether casts should perform runtime checks.
     */
    template <typename T, bool bChecked = false>
        requires std::derived_from<T, UObject> || UnrealInterface<T> || std::derived_from<T, FField>
    struct TDynamicCast {

        /**
         * @brief Functor for casting objects within the Unreal Engine framework.
         *
         * @tparam U The type of the object being cast, must be a derived type of UObject.
         * @param Object Reference to the object to be cast.
         * @return decltype(auto) The casted object or an optional reference to
         *         the casted object depending on template parameters.
         *
         * This operator uses templated type casting to safely cast an object to a
         * specified type within Unreal Engine's object framework.
         *
         * - If U is directly derived from T, it performs a static_cast.
         * - If the checked flag (bChecked) is set, it uses CastChecked to ensure the
         *   cast's validity and throws an error if the cast fails.
         * - Otherwise, it performs a safe cast using Cast, returning an optional
         *   reference to T, which may be null if the cast fails.
         */
        template <typename U>
            requires std::derived_from<U, UObject> || UnrealInterface<U>
                                                  constexpr decltype(auto) operator()(U &Object) const
                         requires std::derived_from<T, UObject>
        {
            if constexpr (std::derived_from<U, T>) {
                // Trivial case, no need to actually cast
                return static_cast<T &>(Object);
            } else if constexpr (bChecked) {
                return *CastChecked<T>(&Object);
            } else {
                return TOptional<T &>(Cast<T>(&Object));
            }
        }

        /**
         * @brief Functor for casting objects within the Unreal Engine framework.
         *
         * @tparam U The type of the object being cast. It must be a derived type of UObject or satisfy
         * UnrealInterface<T> concept.
         * @param Object Reference to the object to be cast.
         * @return The casted object or an optional reference to the casted object depending on the template parameters.
         *
         */
        template <typename U>
            requires std::derived_from<U, UObject> || UnrealInterface<U>
                                                  constexpr decltype(auto) operator()(U &Object) const
                         requires UnrealInterface<T>
        {
            if constexpr (std::derived_from<U, T>) {
                // Trivial case, no need to actually cast
                return TScriptInterface<T>(&Object);
            } else if constexpr (bChecked) {
                if constexpr (std::derived_from<U, UObject>) {
                    check(Object.template Implements<typename T::UClassType>())
                    return TScriptInterface<T>(&Object);
                } else {
                    return *CastChecked<T>(&Object);
                }

            } else {
                if constexpr (std::derived_from<U, UObject>) {
                    if (!Object.template Implements<typename T::UClassType>()) {
                        return TScriptInterface<T>(nullptr);
                    }

                    return TScriptInterface<T>(&Object);
                } else {
                    return TOptional<T &>(Cast<T>(&Object));
                }
            }
        }

        /**
         * @brief Functor for casting objects within the Unreal Engine framework.
         *
         * @tparam U The type of the object being cast. It must be derived from FField.
         * @param Object Reference to the object to be cast.
         * @return The casted object, or an optional reference to the casted object depending on the template
         * parameters.
         *
         * This operator uses templated type casting to safely cast an object to a
         * specified type within Unreal Engine's field framework.
         *
         * - If U is directly derived from T, it performs a static_cast.
         * - If the checked flag (bChecked) is set, it uses CastFieldChecked to ensure the
         *   cast's validity and throws an error if the cast fails.
         * - Otherwise, it performs a safe cast using CastField, returning an optional
         *   reference to T, which may be null if the cast fails.
         */
        template <typename U>
            requires std::derived_from<U, FField>
        constexpr decltype(auto) operator()(U &Object) const
            requires std::derived_from<T, FField>
        {
            if constexpr (std::derived_from<U, T>) {
                // Trivial case, no need to actually cast
                return static_cast<T &>(Object);
            } else if constexpr (bChecked) {
                return *CastFieldChecked<T>(Object);
            } else {
                return TOptional<T &>(CastField<T>(&Object));
            }
        }

        /**
         * @brief Functor for casting pointer objects within the Unreal Engine framework.
         *
         * @tparam U The type of the pointer being cast.
         *           This type must dereference to either a const UObject or a const FField.
         * @param Ptr The pointer to the object that needs to be cast.
         * @return decltype(auto) Returns a reference to the casted object or an optional reference to the casted object
         * depending on the validity of the pointer and the template parameters.
         *
         * This operator checks if the provided pointer is valid. If the checked flag (bChecked) is enabled, it performs
         * a check that throws an error if the pointer is null. If the flag is not enabled and the pointer is invalid,
         * it returns an optional containing null. Otherwise, it returns an optional reference to the casted object.
         */
        template <typename U>
            requires DereferencesTo<U, const UObject> || DereferencesTo<U, const FField>
        constexpr decltype(auto) operator()(U &&Ptr) const {
            using ResultType = decltype(operator()(*Ptr));
            if constexpr (bChecked) {
                checkf(ValidPtr(std::forward<U>(Ptr)),
                       TEXT("Null pointer detected!")) if constexpr (std::is_lvalue_reference_v<ResultType>) {
                    return &operator()(*Ptr);
                }
                else {
                    return operator()(*Ptr);
                }
            } else {
                if (!ValidPtr(std::forward<U>(Ptr))) {
                    return TOptional<T &>(nullptr);
                }

                if constexpr (std::derived_from<std::decay_t<U>, FScriptInterface>) {
                    return operator()(Ptr.GetObject());
                } else {
                    return static_cast<TOptional<T &>>(operator()(*Ptr));
                }
            }
        }

        /**
         * @brief Operator overload to handle pointer validation and conversion.
         *
         * This operator checks the validity of pointers and either returns a reference
         * or a wrapped optional reference based on the input type and the compile-time
         * configuration flag `bChecked`.
         *
         * @tparam U The type of pointer being passed to the operator.
         *
         * @param Ptr A forwarding reference to the pointer to be validated and processed.
         *
         * @return Returns a reference or an optional reference based on the validity check
         * and type of the pointer. If `bChecked` is true and the pointer is invalid, an error
         * is triggered. If `bChecked` is false and the pointer is invalid, a null optional
         * reference is returned.
         */
        template <typename U>
            requires DereferencesToInterface<U>
        constexpr decltype(auto) operator()(U &&Ptr) const {
            if constexpr (bChecked) {
                checkf(
                    ValidPtr(std::forward<U>(Ptr)),
                    TEXT(
                        "Null pointer detected!")) if constexpr (std::derived_from<std::decay_t<U>, FScriptInterface>) {
                    return operator()(Ptr.GetObject());
                }
                else {
                    return &operator()(*Ptr);
                }
            } else {
                if (!ValidPtr(std::forward<U>(Ptr))) {
                    return TOptional<T &>(nullptr);
                }

                if constexpr (std::derived_from<std::decay_t<U>, FScriptInterface>) {
                    return operator()(Ptr.GetObject());
                } else {
                    return static_cast<TOptional<T &>>(operator()(*Ptr));
                }
            }
        }
    };

    /**
     * A constexpr instance of TDynamicCast designed for Unreal Engine objects and fields.
     * Provides utilities for performing dynamic casts with validation, based on template parameters.
     *
     * @tparam T The type to cast to. This type must derive from either UObject, FField, or satisfy the UnrealInterface
     * concept.
     */
    template <typename T>
    constexpr TDynamicCast<T> DynamicCast;

    /**
     * A constexpr instance of TDynamicCast, specialized to enforce runtime checks during casting.
     *
     * Designed for Unreal Engine objects and fields, this variable offers a reliable way to perform dynamic casts
     * with runtime validation, ensuring that the cast is safe and conforms to the expected type.
     *
     * @tparam T The type to cast to. This type must derive from either UObject, FField, or satisfy the UnrealInterface
     * concept.
     */
    template <typename T>
    constexpr TDynamicCast<T, true> DynamicCastChecked;
} // namespace UE::Ranges
