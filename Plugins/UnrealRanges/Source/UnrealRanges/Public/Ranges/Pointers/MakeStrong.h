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

        /**
         * @brief Converts a TWeakObjectPtr to a raw pointer.
         *
         * This operator allows the conversion of a TWeakObjectPtr to a raw pointer of type T. If the bChecked template
         * parameter is true, a runtime check is performed to verify the validity of the weak pointer before conversion.
         *
         * @tparam T The type of object that the weak pointer points to.
         * @param Ptr The TWeakObjectPtr that is to be converted.
         * @return A raw pointer of type T if the weak pointer is valid; otherwise, nullptr.
         */
        template <typename T>
            requires std::derived_from<std::decay_t<T>, UObject>
        constexpr T* operator()(const TWeakObjectPtr<T>& Ptr) const {
            if constexpr (bChecked) {
                check(ValidPtr(Ptr))
            }
            
            return Ptr.Get();
        }

        /**
         * @brief Converts a TWeakInterfacePtr to a TScriptInterface.
         *
         * This operator provides the functionality to convert a TWeakInterfacePtr to a TScriptInterface of type T.
         * If the bChecked template parameter is true, a runtime check is performed to validate the weak pointer before conversion.
         *
         * @tparam T The type of the interface that the weak pointer points to.
         * @param Ptr The TWeakInterfacePtr that is to be converted.
         * @return A TScriptInterface of type T if the weak pointer is valid; otherwise, an invalid TScriptInterface.
         */
        template <typename T>
            requires UnrealInterface<T>
        constexpr TScriptInterface<T> operator()(const TWeakInterfacePtr<T>& Ptr) const {
            if constexpr (bChecked) {
                check(ValidPtr(Ptr))
            }
            
            return Ptr.ToScriptInterface();
        }

        /**
         * @brief Converts a weak pointer to a strong pointer.
         *
         * This operator converts a given weak pointer to its corresponding strong pointer type.
         * If the `bChecked` template parameter is `true`, a runtime check is performed to ensure the
         * weak pointer is valid before conversion.
         *
         * @tparam P The type of the weak pointer being converted.
         * @param Ptr The weak pointer to be converted.
         * @return A strong pointer. If bChecked is true, a checked strong pointer is returned.
         */
        template <typename T, ESPMode M, typename P>
            requires std::same_as<std::decay_t<P>, TWeakPtr<T, M>>
        constexpr auto operator()(P&& Ptr) const {
            if constexpr (bChecked) {
                return std::forward<P>(Ptr).Pin().ToSharedRef();
            } else {
                return std::forward<P>(Ptr).Pin();
            }
        }

        /**
         * @brief Converts a std::weak_ptr to a std::shared_ptr.
         *
         * This operator converts a given std::weak_ptr to its corresponding
         * std::shared_ptr type. If the `bChecked` template parameter is `true`,
         * a runtime check is performed to ensure the weak pointer is valid before conversion.
         *
         * @tparam T The type of the object that the weak pointer points to.
         * @tparam P The type of the weak pointer being converted.
         * @param Ptr The weak pointer to be converted.
         * @return A std::shared_ptr of type T. If bChecked is true, an assertion check is performed.
         */
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
