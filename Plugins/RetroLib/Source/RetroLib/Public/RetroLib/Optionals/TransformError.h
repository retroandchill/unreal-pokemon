// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "RetroLib/Functional/ExtensionMethods.h"
#include "RetroLib/Optionals/OptionalOperations.h"

#ifndef RETROLIB_EXPORT
#define RETROLIB_EXPORT
#endif

namespace Retro::Optionals
{

    template <ExpectedType O, ExpectedType P>
    constexpr auto TransformErrorPassthrough(P &&InputValue)
    {
        return O(Get(std::forward<P>(InputValue)));
    }

    template <template <typename...> typename O, typename T, typename E, typename G>
    constexpr auto TransformErrorHelper(const O<T, E> &, G &&Value)
    {
        return PassError<O<T, std::remove_reference_t<G>>>(std::forward<G>(Value));
    }

    struct FTransformErrorInvoker
    {
        template <ExpectedType O, typename F>
            requires std::invocable<F, TErrorReference<O>>
        constexpr auto operator()(O &&Optional, F &&Functor) const
        {
            using TransformedType = decltype(TransformErrorHelper(
                std::forward<O>(Optional), std::invoke(std::forward<F>(Functor), GetError(std::forward<O>(Optional)))));
            return HasValue(Optional) ? TransformErrorPassthrough<TransformedType>(std::forward<O>(Optional))
                                      : TransformErrorHelper(
                                            std::forward<O>(Optional),
                                            std::invoke(std::forward<F>(Functor), GetError(std::forward<O>(Optional))));
        }
    };

    RETROLIB_EXPORT constexpr auto TransformError = ExtensionMethod<FTransformErrorInvoker{}>;
} // namespace Retro::Optionals
