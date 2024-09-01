// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include <TypeTraits.h>

namespace UE::Ranges {

    template <typename T>
    concept SingleBindDelegate =
        StdExt::IsMemberFunction_v<decltype(&std::remove_cvref_t<T>::Execute)> && requires(T &&Delegate) {
            { Delegate.IsBound() } -> std::same_as<bool>;
        };

    template <typename T>
    concept MulticastDelegate = StdExt::IsMemberFunction_v<decltype(&std::remove_cvref_t<T>::Broadcast)>;

    template <typename T, typename... A>
    concept UEDelegate = SingleBindDelegate<T> || MulticastDelegate<T>;

} // namespace UE::Ranges