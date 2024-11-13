// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Ranges/Concepts/Delegates.h"
#include "Ranges/RangeConcepts.h"
#include <TypeTraits.h>

namespace UE::Ranges {

    template <typename D, typename... A>
        requires UEDelegate<D>
    constexpr decltype(auto) InvokeDelegate(D &&Delegate, A &&...Args) {
        if constexpr (UnicastDelegate<D>) {
            check(Delegate.IsBound())
            return Delegate.Execute(std::forward<A>(Args)...);
        } else if constexpr (MulticastDelegate<D>) {
            Delegate.Broadcast(std::forward<A>(Args)...);
        }
    }

} // namespace UE::Ranges
