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
        struct TIsWeakReferencable<T*> {
            using WeakRefType = TWeakObjectPtr<T>;
        };
        
    }

    template <typename T>
    concept WeakReferenceable = requires {
        typename Detail::TIsWeakReferencable<std::remove_cvref_t<T>>::WeakRefType;
    };
    

    template <typename T>
        requires WeakReferenceable<T>
    using TWeakRefType = typename Detail::TIsWeakReferencable<std::remove_cvref_t<T>>::WeakRefType;
    
}
