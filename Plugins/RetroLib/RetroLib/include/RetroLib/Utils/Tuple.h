/**
 * @file Tuple.h
 * @brief TODO: Fill me out
 *
 * @author Retro & Chill
 * https://github.com/retroandchill
 */

#pragma once

#if !RETROLIB_WITH_MODULES
#include "RetroLib/Concepts/Tuples.h"
#endif

#ifndef RETROLIB_EXPORT
#define RETROLIB_EXPORT
#endif

namespace Retro {
    template <template <typename...> typename C>
    struct ConvertTupleFunction {

        template <template <typename...> typename T, typename... A>
            requires TupleLike<C<std::decay_t<A>...>> && TupleLike<T<A...>>
        constexpr C<std::decay_t<A>...> operator()(T<A...> &Tuple) const {
            return CreateTuple(Tuple, std::make_index_sequence<std::tuple_size_v<C<A...>>>{});
        }

        template <template <typename...> typename T, typename... A>
            requires TupleLike<C<std::decay_t<A>...>> && TupleLike<T<A...>>
        constexpr C<std::decay_t<A>...> operator()(const T<A...> &Tuple) const {
            return CreateTuple(Tuple, std::make_index_sequence<std::tuple_size_v<C<A...>>>{});
        }

        template <template <typename...> typename T, typename... A>
            requires TupleLike<C<A...>> && TupleLike<T<A...>>
        constexpr C<std::decay_t<A>...> operator()(T<std::decay_t<A>...> &&Tuple) const {
            return CreateTuple(std::move(Tuple), std::make_index_sequence<std::tuple_size_v<C<A...>>>{});
        }

      private:
        template <typename T, size_t... N>
        static constexpr auto CreateTuple(T &&Tuple, std::index_sequence<N...>) {
            return C<std::decay_t<std::tuple_element_t<N, std::decay_t<T>>>...>(get<N>(std::forward<T>(Tuple))...);
        }
    };

    RETROLIB_EXPORT template <template <typename...> typename C>
    constexpr ConvertTupleFunction<C> ConvertTuple;
} // namespace retro