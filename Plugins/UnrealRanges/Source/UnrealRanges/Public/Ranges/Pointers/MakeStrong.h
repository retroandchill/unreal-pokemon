// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"

namespace UE::Ranges {
    /**
     * @brief Utility class for converting weak pointers to strong pointers.
     *
     * The TMakeStrong class provides a collection of methods to convert various types of weak pointers
     * (e.g., TWeakObjectPtr, TWeakPtr, std::weak_ptr) to their corresponding strong pointer types.
     * This class is particularly useful when dealing with Unreal Engine's UObject derived classes
     * and standard C++ weak pointers.
     *
     * @tparam bChecked A boolean template parameter that, when set to true, enables runtime checks
     *                  to ensure the weak pointer being converted is valid.
     */
    template <bool bChecked = false>
    struct TMakeStrong {

        template <typename T>
            requires std::derived_from<std::decay_t<T>, UObject>
        constexpr T* operator()(const TWeakObjectPtr<T>& Ptr) const {
            if constexpr (bChecked) {
                check(ValidPtr(Ptr))
            }
            
            return Ptr.Get();
        }

        template <typename T>
            requires UnrealInterface<T>
        constexpr TScriptInterface<T> operator()(const TWeakInterfacePtr<T>& Ptr) const {
            if constexpr (bChecked) {
                check(ValidPtr(Ptr))
            }
            
            return Ptr.ToScriptInterface();
        }

        template <typename T, ESPMode M, typename P>
            requires std::same_as<std::decay_t<P>, TWeakPtr<T, M>>
        constexpr auto operator()(P&& Ptr) const {
            if constexpr (bChecked) {
                return std::forward<P>(Ptr).Pin().ToSharedRef();
            } else {
                return std::forward<P>(Ptr).Pin();
            }
        }

        template <typename T, typename P>
            requires std::same_as<std::decay_t<P>, std::weak_ptr<T>>
        constexpr std::shared_ptr<T> operator()(P &&Ptr) const {
            if constexpr (bChecked) {
                check(!Ptr.expired())
            }
            
            return std::forward<P>(Ptr).lock();
        }
    };

    /**
     * @brief A constant instance of TMakeStrong for converting weak pointers to strong pointers.
     *
     * MakeStrong is a pre-defined instance of the TMakeStrong utility class which provides methods
     * to convert various types of weak pointers (e.g., TWeakObjectPtr, TWeakPtr, std::weak_ptr)
     * to their corresponding strong pointer types without runtime checks.
     *
     * Users can utilize this instance to safely convert weak pointers into strong pointers,
     * facilitating secure pointer management.
     */
    constexpr TMakeStrong<> MakeStrong;

    /**
     * @brief A constant instance of TMakeStrong for converting weak pointers to strong pointers with runtime checks.
     *
     * MakeStrongChecked is a pre-defined instance of the TMakeStrong utility class with runtime checks enabled
     * (`bChecked` set to true). It provides methods to convert various types of weak pointers
     * (e.g., TWeakObjectPtr, TWeakPtr, std::weak_ptr) to their corresponding strong pointer types.
     *
     * When using MakeStrongChecked, runtime checks are performed to ensure that the weak pointer being converted
     * is valid, thereby enhancing the safety and reliability of pointer conversions.
     */
    constexpr TMakeStrong<true> MakeStrongChecked;
}
