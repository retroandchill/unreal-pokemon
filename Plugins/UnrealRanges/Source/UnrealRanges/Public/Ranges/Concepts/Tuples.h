// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include <range/v3/utility/tuple_algorithm.hpp>

namespace UE::Ranges {

    namespace Detail {
        template <typename>
        struct TIsTuple : std::false_type {};

        template <typename T, typename U>
        struct TIsTuple<std::pair<T, U>> : std::true_type {};

        template <typename... T>
        struct TIsTuple<std::tuple<T...>> : std::true_type {};

        template <typename... T>
        struct TIsTuple<TTuple<T...>> : std::true_type {};
    } // namespace Detail

    template <typename T, size_t N>
    concept HasTupleElement = requires(T Tuple) {
        typename std::tuple_element_t<N, std::remove_const_t<T>>;
        { get<N>(Tuple) } -> std::convertible_to<const std::tuple_element_t<N, T> &>;
    };

    template <typename T>
    concept TupleLike = requires(T Tuple) {
        typename std::tuple_size<T>;
        requires std::derived_from<std::tuple_size<T>, std::integral_constant<size_t, std::tuple_size_v<T>>>;
    };

    static_assert(std::tuple_size_v<TTuple<int32, bool, char>> == 3);

    static_assert(TupleLike<std::pair<int32, int32>>);
    static_assert(TupleLike<std::pair<int32, int32>>);
    static_assert(TupleLike<std::tuple<int32, bool, char>>);
    static_assert(TupleLike<TPair<int32, int32>>);
    static_assert(TupleLike<TTuple<int32, bool, char>>);

    template <typename F, typename T>
    concept CanApply = TupleLike<std::remove_reference_t<T>> && requires(std::remove_cvref_t<T> Tuple, F &&Functor) {
        ranges::tuple_apply(Forward<F>(Functor), Tuple);
    };

    static_assert(CanApply<void (*)(int32, int32), std::pair<int32, int32>>);
    static_assert(CanApply<void (*)(int32, int32), TPair<int32, int32>>);

} // namespace UE::Ranges
