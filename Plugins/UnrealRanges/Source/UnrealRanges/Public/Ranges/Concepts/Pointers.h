// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Ranges/Utilities/Unreachable.h"
#include <Ranges/RangeConcepts.h>

namespace UE::Ranges {
    template <typename T>
    concept RawPointer = std::is_pointer_v<T>;

    template <typename T>
    concept StdPointer = requires(T &&Ptr) {
        { Ptr.get() } -> RawPointer;
    };

    template <typename T>
    concept UnrealPointer = requires(T &&Ptr) {
        {  Ptr.Get() } -> RawPointer;
    };

    template <typename T>
    concept Pointer = RawPointer<std::decay_t<T>> || StdPointer<T> || UnrealPointer<T> || std::derived_from<std::decay_t<T>, FScriptInterface>;

    template <typename T>
        requires Pointer<T>
    constexpr auto GetRawPointer(const T &Ptr) {
        if constexpr (RawPointer<T>) {
            return Ptr;
        } else if constexpr (StdPointer<T>) {
            return Ptr.get();
        } else if constexpr (UnrealPointer<T>) {
            return Ptr.Get();
        } else if constexpr (std::same_as<T, FScriptInterface>) {
            return Ptr.GetObject();
        } else if constexpr (std::derived_from<T, FScriptInterface>) {
            return Ptr.GetInterface();
        }

        Unreachable();
    }

    template <typename T>
        requires Pointer<T>
    using TRawPointerType = decltype(GetRawPointer(std::declval<T>()));

    template <typename T>
        requires Pointer<std::remove_cvref_t<T>>
    using TReferenceType = decltype(*std::declval<T>());

    static_assert(Pointer<UObject *>);
    static_assert(Pointer<TObjectPtr<UObject>>);
    static_assert(Pointer<const TObjectPtr<UObject>>);
    static_assert(Pointer<const TObjectPtr<UObject> &>);

    namespace Detail {
        template <typename>
        struct TIsReferenceType : std::false_type {};

        template <typename T>
            requires std::is_base_of_v<UObject, T>
        struct TIsReferenceType<T> : std::true_type {
            using VariableType = TObjectPtr<T>;
            using ArgumentType = T *;
        };

        template <typename T>
            requires UnrealInterface<T>
        struct TIsReferenceType<T> : std::true_type {
            using VariableType = TScriptInterface<T>;
            using ArgumentType = const TScriptInterface<T> &;
        };
    } // namespace Detail

    template <typename T>
    concept UnrealReferenceType = Detail::TIsReferenceType<T>::value;

    template <typename T>
        requires UnrealReferenceType<T>
    using TVariableType = typename TIsReferenceType<T>::VariableType;

    namespace Detail {
        template <typename T>
        struct TDecayReference {
            using Type = std::decay_t<T>;
        };

        template <typename T>
            requires std::is_lvalue_reference_v<T>
        struct TDecayReference<T> {
            using Type = std::remove_reference_t<T> *;
        };
    } // namespace Detail

    template <typename T>
    using TDecayReferenceType = typename Detail::TDecayReference<T>::Type;

    template <typename T, typename U>
    concept DereferencesTo = requires(T&& Ptr) {
        { *Ptr } -> std::convertible_to<U&>;
    };

    template <typename T>
    concept DereferencesToInterface  = requires(T &&Ptr) {
        { *Ptr } -> UnrealInterface;
    };
} // namespace UE::Ranges