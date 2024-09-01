﻿// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"

namespace UE::Ranges {
    namespace Detail {
        template <typename>
        struct TPointerTraits;

        template <typename T>
        struct TPointerTraits<T *> {
            using RawPointer = T *;

            FORCEINLINE static T *GetRawPointer(T *Ptr) {
                return Ptr;
            }
        };

        template <typename T>
        struct TPointerTraits<TObjectPtr<T>> {
            using RawPointer = T *;

            FORCEINLINE static T *GetRawPointer(const TObjectPtr<T> &Ptr) {
                return Ptr;
            }
        };

        template <typename T>
        struct TPointerTraits<TScriptInterface<T>> {
            using RawPointer = T *;

            FORCEINLINE static T *GetRawPointer(const TScriptInterface<T> &Ptr) {
                return Ptr.GetInterface();
            }
        };

        template <typename T>
        struct TPointerTraits<TUniquePtr<T>> {
            using RawPointer = T *;

            FORCEINLINE static T *GetRawPointer(const TUniquePtr<T> &Ptr) {
                return Ptr.Get();
            }
        };

        template <typename T>
        struct TPointerTraits<TSharedRef<T>> {
            using RawPointer = T *;

            FORCEINLINE static T *GetRawPointer(const TSharedRef<T> &Ptr) {
                return Ptr.Get();
            }
        };

        template <typename T>
        struct TPointerTraits<TSharedPtr<T>> {
            using RawPointer = T *;

            FORCEINLINE static T *GetRawPointer(const TSharedPtr<T> &Ptr) {
                return Ptr.Get();
            }
        };
    } // namespace Detail

    template <typename T>
    concept RawPointer = std::is_pointer_v<T>;

    template <typename T>
    concept Pointer = std::is_pointer_v<typename Detail::TPointerTraits<std::remove_cvref_t<T>>::RawPointer> &&
                      requires(const std::remove_cvref_t<T> &Ptr) {
                          { Detail::TPointerTraits<std::remove_cvref_t<T>>::GetRawPointer(Ptr) } -> RawPointer;
                      };

    template <typename T>
        requires Pointer<std::remove_cvref_t<T>>
    FORCEINLINE auto GetRawPointer(const std::remove_cvref_t<T> &Ptr) {
        return Detail::TPointerTraits<std::remove_cvref_t<T>>::GetRawPointer(Ptr);
    }

    template <typename T>
    using TRawPointerType = typename Detail::TPointerTraits<std::remove_cvref_t<T>>::RawPointer;

    static_assert(Pointer<UObject *>);
    static_assert(Pointer<TObjectPtr<UObject>>);
    static_assert(Pointer<const TObjectPtr<UObject>>);
    static_assert(Pointer<const TObjectPtr<UObject> &>);
} // namespace UE::Ranges