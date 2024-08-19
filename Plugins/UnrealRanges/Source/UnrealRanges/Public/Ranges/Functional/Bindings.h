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
    constexpr auto CreateBinding(F&& Functor, A&&... Args) {
        if constexpr (sizeof...(A) > 0) {
            return ranges::bind_back(Forward<F>(Functor), Forward<A>(Args)...);
        } else {
            return MoveTempIfPossible(Functor);
        }
    }

    template <typename C, typename F, typename... A>
        requires StdExt::IsMemberFunction_v<F>
    constexpr auto CreateBinding(C&& Object, F&& Functor, A&&... Args) {
        if constexpr (std::is_base_of_v<StdExt::MemberFunctionClass_t<F>, std::remove_cvref_t<C>>) {
            return std::bind_front(CreateBinding(Forward<F>(Functor), Forward<A>(Args)...), &Object);
        } else {
            return std::bind_front(CreateBinding(Forward<F>(Functor), Forward<A>(Args)...), Forward<C>(Object));
        }
    }

    template <typename M>
        requires std::is_member_pointer_v<M> && (!std::is_member_function_pointer_v<M>)
    constexpr auto CreateBinding(M&& Member) {
        return Forward<M>(Member);
    }
    
}