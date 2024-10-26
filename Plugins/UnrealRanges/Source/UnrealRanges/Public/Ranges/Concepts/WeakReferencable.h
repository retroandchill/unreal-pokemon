// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"

namespace UE::Ranges {

    namespace Detail {

        template <typename>
        struct TIsWeakReferencable {};

        template <typename T>
        struct TIsWeakReferencable<TSharedRef<T>> {
            using WeakRefType = TWeakPtr<T>;
        };

        template <typename T>
        struct TIsWeakReferencable<TSharedPtr<T>> {
            using WeakRefType = TWeakPtr<T>;
        };

        template <typename T>
        struct TIsWeakReferencable<TObjectPtr<T>> {
            using WeakRefType = TWeakObjectPtr<T>;
        };

        template <typename T>
            requires std::is_base_of_v<UObject, T>
        struct TIsWeakReferencable<T *> {
            using WeakRefType = TWeakObjectPtr<T>;
        };

        template <typename>
        struct TIsWeakReference : std::false_type {};

        template <typename T>
        struct TIsWeakReference<TWeakPtr<T>> : std::true_type {

            static TSharedPtr<T> ToStrong(const TWeakPtr<T> &Ptr) {
                return Ptr.Pin();
            }
        };

        template <typename T>
            requires std::is_base_of_v<UObject, T>
        struct TIsWeakReference<TWeakObjectPtr<T>> : std::true_type {

            static T *ToStrong(const TWeakObjectPtr<T> &Ptr) {
                return Ptr.Get();
            }
        };

    } // namespace Detail

    template <typename T>
    concept WeakReferenceable = requires { typename Detail::TIsWeakReferencable<std::remove_cvref_t<T>>::WeakRefType; };

    template <typename T>
        requires WeakReferenceable<T>
    using TWeakRefType = typename Detail::TIsWeakReferencable<std::remove_cvref_t<T>>::WeakRefType;

    template <typename T>
    concept WeakReference = Detail::TIsWeakReference<std::remove_cvref_t<T>>::value;

    /**
     * Convert a weak pointer type into its strong reference version.
     * @tparam T The type of pointer to make strong.
     * @param Ptr The pointer to make strong
     * @return The new pointer
     */
    template <typename T>
        requires WeakReference<T>
    FORCEINLINE auto ToStrongRef(T &&Ptr) {
        return Detail::TIsWeakReference<std::remove_cvref_t<T>>::ToStrong(std::forward<T>(Ptr));
    }

} // namespace UE::Ranges
