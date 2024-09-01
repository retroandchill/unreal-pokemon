// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Ranges/Concepts/Delegates.h"

namespace UE::Ranges {

    template <typename D, typename... A>
        requires UEDelegate<D>
    constexpr auto InvokeDelegate(D&& Delegate, A&&... Args) {
        if constexpr (SingleBindDelegate<D>) {
            check(Delegate.IsBound())
            return Delegate.Execute(Forward<A>(Args)...);
        } else if constexpr (MulticastDelegate<D>) {
            Delegate.Broadcast(Forward<A>(Args)...);
        }
    }
    
}