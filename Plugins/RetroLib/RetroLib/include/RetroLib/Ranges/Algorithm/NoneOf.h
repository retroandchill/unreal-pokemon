/**
 * @file NoneOf.h
 * @brief Functional binding helper for the none_of range operation.
 *
 * @author Retro & Chill
 * https://github.com/retroandchill
 */

#pragma once

#if !RETROLIB_WITH_MODULES
#include "Retrolib/Functional/CreateBinding.h"
#include "Retrolib/Functional/ExtensionMethods.h"
#include "Retrolib/Ranges/FeatureBridge.h"

#include <ranges>
#endif

#ifndef RETROLIB_EXPORT
#define RETROLIB_EXPORT
#endif

namespace retro::ranges {
    RETROLIB_EXPORT template <std::ranges::input_range R, typename... A>
        requires std::invocable<BindingType<A...>, RangeCommonReference<R>>
    constexpr bool none_of(R &&range, A &&...args) {
        return std::ranges::none_of(std::forward<R>(range), create_binding(std::forward<A>(args)...));
    }

    RETROLIB_EXPORT template <auto Functor, std::ranges::input_range R, typename... A>
        requires HasFunctionCallOperator<decltype(Functor)> &&
                 std::invocable<ConstBindingType<Functor, A...>, RangeCommonReference<R>>
    constexpr bool none_of(R &&range, A &&...args) {
        return std::ranges::none_of(std::forward<R>(range), create_binding<Functor>(std::forward<A>(args)...));
    }

    struct NoneOfInvoker {
        template <std::ranges::input_range R, typename... A>
            requires std::invocable<BindingType<A...>, RangeCommonReference<R>>
        constexpr bool operator()(R &&range, A &&...args) const {
            return none_of(std::forward<R>(range), std::forward<A>(args)...);
        }
    };

    constexpr NoneOfInvoker none_of_invoker;

    template <auto Functor>
        requires HasFunctionCallOperator<decltype(Functor)>
    struct NoneOfConstInvoker {
        template <std::ranges::input_range R, typename... A>
            requires std::invocable<ConstBindingType<Functor, A...>, RangeCommonReference<R>>
        constexpr bool operator()(R &&range, A &&...args) const {
            return none_of<Functor>(std::forward<R>(range), std::forward<A>(args)...);
        }
    };

    template <auto Functor>
        requires HasFunctionCallOperator<decltype(Functor)>
    constexpr NoneOfConstInvoker<Functor> none_of_const_invoker;

    RETROLIB_EXPORT template <typename... A>
    constexpr auto none_of(A &&...args) {
        return extension_method<none_of_invoker>(std::forward<A>(args)...);
    }

    RETROLIB_EXPORT template <auto Functor, typename... A>
        requires HasFunctionCallOperator<decltype(Functor)>
    constexpr auto none_of(A &&...args) {
        return extension_method<none_of_const_invoker<Functor>>(std::forward<A>(args)...);
    }
} // namespace retro::ranges