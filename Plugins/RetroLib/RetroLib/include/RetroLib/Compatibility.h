/**
 * @file Compatibility.h
 * @brief Compatibility patches for using modules
 *
 * @author Retro & Chill
 * https://github.com/retroandchill
 */

#pragma once

#if !RETROLIB_WITH_MODULES
#include <type_traits>
#include "RetroLib/Concepts/Tuples.h"
#endif

#ifndef RETROLIB_EXPORT
#define RETROLIB_EXPORT
#endif

#if __cplusplus < 202302L
namespace std {

    RETROLIB_EXPORT template <retro::TupleLike T, retro::TupleLike U, template <typename> typename A,
                              template <typename> typename B>
        requires(retro::SpecializationOf<T, tuple> || retro::SpecializationOf<U, tuple>) && is_same_v<T, decay_t<T>> &&
                is_same_v<U, decay_t<U>> && (tuple_size_v<T> == tuple_size_v<U>) &&
                requires { typename retro::TupleLikeCommonReference<T, U, A, B>::Type; }
    struct basic_common_reference<T, U, A, B> {
        using type = typename retro::TupleLikeCommonReference<T, U, A, B>::Type;
    };

    RETROLIB_EXPORT template <retro::TupleLike T, retro::TupleLike U>
        requires(retro::SpecializationOf<T, tuple> || retro::SpecializationOf<U, tuple>) && is_same_v<T, decay_t<T>> &&
                is_same_v<U, decay_t<U>> && (tuple_size_v<T> == tuple_size_v<U>) &&
                requires { typename retro::TupleLikeCommonType<T, U>::Type; }
    struct common_type<T, U> {
        using type = typename retro::TupleLikeCommonType<T, U>::Type;
    };
} // namespace std
#endif