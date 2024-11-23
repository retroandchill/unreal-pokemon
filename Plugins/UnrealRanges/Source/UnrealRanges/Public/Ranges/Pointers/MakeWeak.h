// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Ranges/Concepts/Pointers.h"

namespace UE::Ranges {
    /**
     * @class FMakeWeak
     *
     * @brief A utility class to create and manage weak pointers.
     *
     * The FMakeWeak class provides functionality to create weak pointers
     * from shared pointers or UObject pointers, enabling easy management of object lifetimes
     * without creating ownership cycles.
     */
    struct FMakeWeak {

        template <typename T>
            requires std::derived_from<T, UObject>
        constexpr TWeakObjectPtr<T> operator()(T *Ptr) const {
            return TWeakObjectPtr<T>(Ptr);
        }

        template <typename T>
            requires std::derived_from<T, UObject>
        constexpr TWeakObjectPtr<T> operator()(const TObjectPtr<T> &Ptr) const {
            return TWeakObjectPtr<T>(Ptr);
        }

        template <typename T>
            requires std::derived_from<T, UObject>
        constexpr TWeakObjectPtr<T> operator()(const TStrongObjectPtr<T> &Ptr) const {
            return TWeakObjectPtr<T>(Ptr.Get());
        }

        template <typename T>
            requires UnrealInterface<T>
        constexpr TWeakInterfacePtr<T> operator()(T *Ptr) const {
            return TWeakInterfacePtr<T>(Ptr);
        }

        template <typename T>
            requires UnrealInterface<T>
        constexpr TWeakInterfacePtr<T> operator()(const TScriptInterface<T> &Ptr) const {
            return TWeakInterfacePtr<T>(Ptr);
        }

        template <typename T, ESPMode M>
        constexpr TWeakPtr<T> operator()(const TSharedRef<T, M> &Ptr) const {
            return Ptr.ToWeakPtr();
        }

        template <typename T, ESPMode M>
        constexpr TWeakPtr<T> operator()(const TSharedPtr<T, M> &Ptr) const {
            return Ptr.ToWeakPtr();
        }

        template <typename T>
        constexpr std::weak_ptr<T> operator()(const std::shared_ptr<T> &Ptr) const {
            return std::weak_ptr<T>(Ptr);
        }
    };

    /**
     * @brief An instance of the FMakeWeak utility class.
     *
     * The MakeWeak object provides a convenient mechanism to create and manage weak pointers
     * from a variety of strong pointer types. It helps in managing object lifetimes
     * and avoiding ownership cycles by transforming shared pointers, UObject pointers,
     * and other pointer types to their corresponding weak pointer counterparts.
     */
    constexpr FMakeWeak MakeWeak;
} // namespace UE::Ranges
