// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"

namespace UE::Ranges {
    /**
     * @brief A utility struct to wrap raw pointers into appropriate Unreal Engine pointer types.
     *
     * This struct provides a call operator that can be used to wrap raw pointers of UObject-derived
     * types or Unreal Engine interface types into their respective smart pointer types.
     */
    struct FWrapPointer {
        /**
         * @brief Wraps raw pointers of UObject-derived types or Unreal Engine interface types into their respective
         * smart pointer types.
         *
         * This operator allows conversion of raw pointers to appropriate Unreal Engine smart pointers, ensuring proper
         * memory management.
         *
         * @tparam T The type of the object being pointed to, which must be either a UObject-derived type or an Unreal
         * Engine interface.
         * @param Ptr The raw pointer to the object.
         * @return TObjectPtr<T> if T is derived from UObject, or TScriptInterface<T> if T implements an Unreal Engine
         * interface.
         */
        template <typename T>
            requires std::derived_from<std::remove_cv_t<T>, UObject> || UnrealInterface<T>
        constexpr auto operator()(T *Ptr) const {
            if constexpr (std::derived_from<std::remove_cv_t<T>, UObject>) {
                return TObjectPtr<T>(Ptr);
            } else if constexpr (UnrealInterface<T>) {
                TScriptInterface<T> Interface;
                Interface.SetObject(Ptr != nullptr ? Ptr->_getUObject() : nullptr);
                Interface.SetInterface(Ptr);
                return Interface;
            }
        }
    };

    /**
     * @brief An instance of FWrapPointer for wrapping raw pointers into Unreal Engine smart pointer types.
     *
     * This constant leverages the functionality of FWrapPointer to provide an easy-to-use mechanism for converting raw
     * pointers of UObject-derived types or Unreal Engine interface types into their respective smart pointer types.
     *
     * @see FWrapPointer
     */
    constexpr FWrapPointer WrapPointer;
} // namespace UE::Ranges
