// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "range/v3/functional/bind_back.hpp"
#include "Ranges/RangeConcepts.h"

namespace UE::Ranges {

    /**
     * Bind the supplied functor with the rear-most arguments being bound to the specified values. This works similarly
     * to ranges::bind_back except that it can work with no arguments supplied, which will simply return the functor
     * back to the caller.
     * @tparam F The functor to bind back to.
     * @tparam A The types of the arguments
     * @param Functor The functor to bind
     * @param Args The arguments to bind
     * @return The bound functor
     */
    template <typename F, typename... A>
        requires FunctionalType<F>
    FORCEINLINE constexpr auto BindFunctor(F&& Functor, A&&... Args) {
        if constexpr (sizeof...(A) > 0) {
            return ranges::bind_back(Forward<F>(Functor), Forward<A>(Args)...);
        } else {
            return MoveTempIfPossible(Functor);
        }
    }
    
}