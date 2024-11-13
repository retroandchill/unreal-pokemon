// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "range/v3/functional/bind_back.hpp"
#include "Ranges/Concepts/Delegates.h"
#include "Ranges/Functional/DelegateBinding.h"
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
    constexpr decltype(auto) CreateBinding(F &&Functor, A &&...Args) {
        if constexpr (sizeof...(A) > 0) {
            return ranges::bind_back(std::forward<F>(Functor), std::forward<A>(Args)...);
        } else {
            return std::forward<F>(Functor);
        }
    }

    template <typename C, typename F, typename... A>
        requires StdExt::IsMemberFunction_v<F>
    constexpr decltype(auto) CreateBinding(C &&Object, F &&Functor, A &&...Args) {
        if constexpr (std::is_base_of_v<StdExt::MemberFunctionClass_t<F>, std::remove_cvref_t<C>>) {
            return std::bind_front(CreateBinding(std::forward<F>(Functor), std::forward<A>(Args)...), &Object);
        } else {
            return std::bind_front(CreateBinding(std::forward<F>(Functor), std::forward<A>(Args)...),
                                   std::forward<C>(Object));
        }
    }

    template <typename M>
        requires std::is_member_pointer_v<M> && (!std::is_member_function_pointer_v<M>)
    constexpr decltype(auto) CreateBinding(M &&Member) {
        return std::forward<M>(Member);
    }

    template <typename D, typename... A>
        requires UEDelegate<D>
    constexpr decltype(auto) CreateBinding(D &&Delegate, A &&...Args) {
        return ranges::bind_back([Callback = std::forward<D>(Delegate)]<typename... B>(
                                     B &&...Vals) { return InvokeDelegate(Callback, std::forward<B>(Vals)...); },
                                 std::forward<A>(Args)...);
    }

    template <typename T, typename... A>
    concept CanInvokeBinding = std::invocable<decltype(CreateBinding<A...>(std::forward<A>(std::declval<A>())...)), T>;

} // namespace UE::Ranges