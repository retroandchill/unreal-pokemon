/**
 * @file BindFunctor.h
 * @brief Creates a simple binding to a functor type.
 *
 * @author Retro & Chill
 * https://github.com/retroandchill
 */
#pragma once

#if !RETROLIB_WITH_MODULES
#include "RetroLib/Concepts/ParameterPacks.h"
#include "RetroLib/FunctionTraits.h"
#include "RetroLib/Utils/ForwardLike.h"

#include <tuple>
#endif

#ifndef RETROLIB_EXPORT
#define RETROLIB_EXPORT
#endif

namespace Retro {

    /**
     * @struct SimpleFunctorBinding
     * @brief A struct that encapsulates a functor and provides an operator() to invoke it.
     *
     * SimpleFunctorBinding provides a call operator to invoke a stored callable object
     * (functor) with the specified arguments. This struct uses a template to ensure
     * that the functor can be called with the provided argument types.
     *
     * @tparam Functor A callable constexpr value.
     *
     * The operator() uses std::invoke to call the Functor with forwarded arguments,
     * ensuring perfect forwarding of the argument types.
     *
     * @note This struct requires the Functor to be invocable with the argument types
     * specified in the call to operator().
     */
    template <auto Functor>
        requires HasFunctionCallOperator<decltype(Functor)>
    struct SimpleFunctorBinding {
        using F = decltype(Functor);

        /**
         * @brief Invokes a specified functor with the provided arguments.
         *
         * This operator overload allows an instance of the containing class to be called like a function,
         * forwarding the provided arguments to a specified functor using `std::invoke`.
         *
         * @tparam A Template parameter pack representing the types of arguments being forwarded.
         * @param Args An arbitrary number of arguments to be forwarded to the functor.
         * @return The result of invoking the functor with the given arguments.
         */
        template <typename... A>
            requires std::invocable<F, A...>
        constexpr decltype(auto) operator()(A &&...Args) const noexcept(std::is_nothrow_invocable_v<F, A...>) {
            return std::invoke(Functor, std::forward<A>(Args)...);
        }
    };

    /**
     * Binds a functor to a simple functor binding instance.
     *
     * @return A SimpleFunctorBinding instance associated with the specified Functor.
     */
    template <auto Functor>
        requires HasFunctionCallOperator<decltype(Functor)>
    constexpr auto BindFunctor() {
        return SimpleFunctorBinding<Functor>();
    }

} // namespace retro