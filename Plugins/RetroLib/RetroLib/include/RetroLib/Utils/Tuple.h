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

namespace retro {
    template <template <typename...> typename C>
    struct ConvertTuple {

        template <template <typename...> typename T, typename... A>
            requires TupleLike<C<std::decay_t<A>...>> && TupleLike<T<A...>>
        constexpr C<std::decay_t<A>...> operator()(T<A...> &tuple) const {
            return create_tuple(tuple, std::make_index_sequence<std::tuple_size_v<C<A...>>>{});
        }

        template <template <typename...> typename T, typename... A>
            requires TupleLike<C<std::decay_t<A>...>> && TupleLike<T<A...>>
        constexpr C<std::decay_t<A>...> operator()(const T<A...> &tuple) const {
            return create_tuple(tuple, std::make_index_sequence<std::tuple_size_v<C<A...>>>{});
        }

        template <template <typename...> typename T, typename... A>
            requires TupleLike<C<A...>> && TupleLike<T<A...>>
        constexpr C<std::decay_t<A>...> operator()(T<std::decay_t<A>...> &&tuple) const {
            return create_tuple(std::move(tuple), std::make_index_sequence<std::tuple_size_v<C<A...>>>{});
        }

      private:
        template <typename T, size_t... N>
        static constexpr auto create_tuple(T &&tuple, std::index_sequence<N...>) {
            return C<std::decay_t<std::tuple_element_t<N, std::decay_t<T>>>...>(get<N>(std::forward<T>(tuple))...);
        }
    };

    RETROLIB_EXPORT template <template <typename...> typename C>
    constexpr ConvertTuple<C> convert_tuple;
    ;
} // namespace retro