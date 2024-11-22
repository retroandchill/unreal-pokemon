// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Ranges/Utilities/Unreachable.h"
#include <Ranges/RangeConcepts.h>

namespace UE::Ranges {
    /**
     * Checks if a type is a raw pointer.
     *
     * @tparam T The type to check
     */
    template <typename T>
    concept RawPointer = std::is_pointer_v<T>;

    /**
     * Checks if a type is smart pointer type in the STL.
     *
     * @tparam T The type to check
     */
    template <typename T>
    concept StdPointer = requires(T &&Ptr) {
        { Ptr.get() } -> RawPointer;
    };

    /**
     * Checks if a type is pointer type in Unreal Engine.
     *
     * @tparam T The type to check
     */
    template <typename T>
    concept UnrealPointer = requires(T &&Ptr) {
        {  Ptr.Get() } -> RawPointer;
    };

    /**
     * Checks if a type is some type of pointer.
     *
     * @tparam T The type to check
     */
    template <typename T>
    concept Pointer = RawPointer<std::decay_t<T>> || StdPointer<T> || UnrealPointer<T> || std::derived_from<std::decay_t<T>, FScriptInterface>;

    // We want to disable unreachable warnings because this function can fail for some reason
    PRAGMA_DISABLE_UNREACHABLE_CODE_WARNINGS
    
    /**
     * Returns the raw pointer from a given pointer type. The pointer type can be a raw pointer,
     * a standard library smart pointer, or an Unreal Engine pointer type.
     *
     * @param Ptr The pointer object from which to retrieve the raw pointer.
     *            The type of this parameter should be one of the following:
     *            - Raw pointer
     *            - Standard library smart pointer (e.g., std::unique_ptr, std::shared_ptr)
     *            - Unreal Engine smart pointer (e.g., TSharedPtr, TWeakPtr)
     *            - FScriptInterface
     *
     * @return The raw pointer of type T. If the input pointer type is not recognized, the function will call Unreachable().
     */
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
    PRAGMA_RESTORE_UNREACHABLE_CODE_WARNINGS

    /**
     * The underlying pointer type of a wrapped pointer type.
     *
     * @tparam T The pointer type
     */
    template <typename T>
        requires Pointer<T>
    using TRawPointerType = decltype(GetRawPointer(std::declval<T>()));

    /**
     * The reference type obtained by dereferencing a pointer type.
     *
     * @tparam T the pointer type
     */
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

    /**
     * Checks if the type is an Unreal Engine reference type.
     *
     * @tparam T The type to check
     */
    template <typename T>
    concept UnrealReferenceType = Detail::TIsReferenceType<T>::value;

    /**
     * The underlying type for a member variable for a reference type.
     *
     * @tparam T The reference type
     */
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

    /**
     * Slightly different version of std::decay_t that turns a refrence into a pointer instead of its value type.
     *
     * @tparam T The type to decay
     */
    template <typename T>
    using TDecayReferenceType = typename Detail::TDecayReference<T>::Type;

    /**
     * Checks if the dereference operator returns a reference to the underlying type.
     *
     * @tparam T The pointer type
     * @tparam U The target type
     */
    template <typename T, typename U>
    concept DereferencesTo = requires(T&& Ptr) {
        { *Ptr } -> std::convertible_to<U&>;
    };

    /**
     * Checks if the type in question is a reference to an interface.
     *
     * @tparam T The reference type
     */
    template <typename T>
    concept InterfaceReference = std::is_reference_v<T> && UnrealInterface<std::remove_reference_t<T>>;

    /**
     * Checks if the dereference operator returns a reference to an Unreal Engine interface.
     *
     * @tparam T The pointer type
     */
    template <typename T>
    concept DereferencesToInterface  = requires(T &&Ptr) {
        { *Ptr } -> InterfaceReference;
    };
} // namespace UE::Ranges