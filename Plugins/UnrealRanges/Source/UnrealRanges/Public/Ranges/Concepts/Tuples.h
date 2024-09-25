// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"

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
    }

    template <typename T, size_t N>
    concept HasTupleElement = requires(T Tuple) {
      typename std::tuple_element_t<N, std::remove_const_t<T>>;
      { get<N>(Tuple) } -> std::convertible_to<const std::tuple_element_t<N, T>&>;
    };

    template <typename T>
    concept TupleLike = requires(T Tuple) {
        typename std::tuple_size<T>;
        requires std::derived_from<std::tuple_size<T>, std::integral_constant<size_t, std::tuple_size_v<T>>>;
    } && []<size_t... N>(std::index_sequence<N...>) {
        return (HasTupleElement<T, N> && ...);
    }(std::make_index_sequence<std::tuple_size_v<T>>());

    static_assert(TupleLike<std::pair<int32, int32>>);
    static_assert(TupleLike<std::pair<int32, int32>>);
    static_assert(TupleLike<std::tuple<int32, bool, char>>);
    static_assert(TupleLike<TPair<int32, int32>>);
    static_assert(TupleLike<TTuple<int32, bool, char>>);

    template <typename F, typename T>
    concept CanApply = TupleLike<std::remove_cvref_t<T>> && requires(T&& Tuple, F&& Functor) {
        { std::apply(Forward<F>(Functor), Forward<T>(Tuple)) } -> std::convertible_to<decltype([
            Functor, Tuple]<size_t... N>(std::index_sequence<N...>) {
                return Functor(std::get<N>(Tuple)...);
            }(std::make_index_sequence<std::tuple_size_v<std::remove_cvref_t<T>>>()))>;
    };

    static_assert(CanApply<void(*)(int32, int32), std::pair<int32, int32>>);
    static_assert(CanApply<void(*)(int32, int32), std::pair<int32, int32>&>);
    
}
