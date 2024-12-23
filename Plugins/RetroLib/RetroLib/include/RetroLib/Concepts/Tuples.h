/**
 * @file Tuples.h
 * @brief Concepts pertaining to tuples.
 *
 * @author Retro & Chill
 * https://github.com/retroandchill
 */
#pragma once

#if !RETROLIB_WITH_MODULES
#include "RetroLib/FunctionTraits.h"
#include "RetroLib/Utils/ForwardLike.h"

#include <tuple>
#include <type_traits>
#endif

#ifndef RETROLIB_EXPORT
#define RETROLIB_EXPORT
#endif

namespace retro {
    /**
     * The type of forwarded tuple element. Normally it's the same as using `forward_like`, except when the tuple
     * element is an l-value reference, where the reference is retained.
     *
     * @tparam T The tuple index
     * @tparam I The index to look up
     */
    RETROLIB_EXPORT template <typename T, size_t I>
    using ForwardedTupleType = std::conditional_t<std::is_lvalue_reference_v<std::tuple_element_t<I, std::decay_t<T>>>,
                                                  std::tuple_element_t<I, std::decay_t<T>>,
                                                  ForwardLikeType<T, std::tuple_element_t<I, std::decay_t<T>>>>;

    /**
     * Concept to check if a type is indexable by the given tuple element.
     *
     * @tparam T The candidate tuple type
     * @tparam I The index to try and get
     */
    RETROLIB_EXPORT template <typename T, size_t I>
    concept HasTupleElement = requires(T &&t) {
        { get<I>(std::forward<T>(t)) } -> std::convertible_to<std::tuple_element_t<I, T>>;
    } || requires(T t) {
        { std::get<I>(std::forward<T>(t)) } -> std::convertible_to<std::tuple_element_t<I, T>>;
    };

    /**
     * Evaluates at compile-time if all specified indices in a given index sequence
     * have an associated tuple element defined for the type `T`.
     *
     * @param std::index_sequence<I...> A compile-time index sequence.
     *            It contains a sequence of indices that will be checked to ensure each
     *            corresponds to a valid tuple element in `T`.
     *
     * @return A compile-time boolean value indicating whether all indices in the index
     *         sequence correspond to valid tuple elements within the type `T`. Returns true
     *         if all indices*/
    RETROLIB_EXPORT template <typename T, size_t... I>
    consteval bool all_has_tuple_element(std::index_sequence<I...>) {
        return (HasTupleElement<T, I> && ...);
    }

    /**
     * Concept to verify if a type is like a tuple or not (and hence can be used with structured bindings and
     * std::apply).
     *
     * @tparam T The type to check the tuple status against
     */
    RETROLIB_EXPORT template <typename T>
    concept TupleLike = requires {
        typename std::tuple_size<T>::type;
        { std::tuple_size_v<T> } -> std::convertible_to<size_t>;
    } && all_has_tuple_element<T>(std::make_index_sequence<std::tuple_size_v<T>>{});

    /**
     * Checks if the given args can be applied to the given functor type.
     *
     * @tparam F The functional type
     * @tparam T The tuple type
     * @param I The indicies that are to be applied as arguments
     */
    template <typename F, typename T, size_t... I>
    concept CanApplyArgs = HasFunctionCallOperator<std::decay_t<F>> && TupleLike<std::decay_t<T>> &&
                           std::invocable<F, ForwardedTupleType<T, I>...>;

    /**
     * Determines if the template arguments can be applied with an index sequence.
     *
     * @param std::index_sequence<I...> An index sequence of integers used to check
     *                                  applicability of template arguments.
     * @return bool Returns true if the template arguments can be successfully applied with
     *              the given index sequence, otherwise false.
     */
    template <typename F, typename T, size_t... I>
    consteval bool can_apply_index_sequence(std::index_sequence<I...>) {
        return CanApplyArgs<F, T, I...>;
    }

    /**
     * Checks if the tuple can be applied to the given functional callback using `std::apply`.
     *
     * @tparam F The functional type
     * @tparam T The tuple type
     */
    RETROLIB_EXPORT template <typename F, typename T>
    concept CanApply = HasFunctionCallOperator<std::decay_t<F>> && TupleLike<std::decay_t<T>> &&
                       can_apply_index_sequence<F, T>(std::make_index_sequence<std::tuple_size_v<std::decay_t<T>>>{});

    /**
     * Checks if the given args when applied to the given functor type is noexcept.
     *
     * @tparam F The functional type
     * @tparam T The tuple type
     * @param I The indicies that are to be applied as arguments
     */
    template <typename F, typename T, size_t... I>
    concept NoThrowApplicableArgs = HasFunctionCallOperator<std::decay_t<F>> && TupleLike<std::decay_t<T>> &&
                                    std::is_nothrow_invocable_v<F, ForwardedTupleType<T, I>...>;

    /**
     * @brief Determines whether a given set of arguments can be applied to a callable
     * object without throwing exceptions at compile-time.
     *
     * @tparam F The callable object to check.
     * @tparam T The type arguments to apply to the callable object.
     * @param[in] std::index_sequence<I...> The sequence of indices representing the
     * positional argument indices used for the check.
     * @retval true If the callable object can be applied with the given arguments
     *         without throwing exceptions.
     * @retval false If the callable object cannot be applied without throwing
     *         exceptions.
     */
    template <typename F, typename T, size_t... I>
    consteval bool is_nothrow_applicable(std::index_sequence<I...>) {
        return NoThrowApplicableArgs<F, T, I...>;
    }
    /**
     * Checks if the given args when applied to the given functor type is noexcept.
     *
     * @tparam F The functional type
     * @tparam T The tuple type
     */
    RETROLIB_EXPORT template <typename F, typename T>
    concept NoThrowApplicable =
        HasFunctionCallOperator<std::decay_t<F>> && TupleLike<std::decay_t<T>> &&
        is_nothrow_applicable<F, T>(std::make_index_sequence<std::tuple_size_v<std::decay_t<T>>>{});

    RETROLIB_EXPORT template <TupleLike T, TupleLike U, template <typename> typename A, template <typename> class B,
                              typename I = std::make_index_sequence<std::tuple_size_v<T>>>
    struct TupleLikeCommonReference;

    template <typename T, typename U, template <typename> typename A, template <typename> typename B, size_t... I>
    concept TupleLikeCommonReferenceHelper = requires {
        typename std::tuple<std::common_reference_t<A<std::tuple_element_t<I, T>>, B<std::tuple_element_t<I, U>>>...>;
    };

    RETROLIB_EXPORT template <typename T, typename U, template <typename> typename A, template <typename> typename B,
                              size_t... I>
        requires TupleLikeCommonReferenceHelper<T, U, A, B, I...>
    struct TupleLikeCommonReference<T, U, A, B, std::index_sequence<I...>> {
        using Type =
            std::tuple<std::common_reference_t<A<std::tuple_element_t<I, T>>, B<std::tuple_element_t<I, U>>>...>;
    };

    RETROLIB_EXPORT template <TupleLike T, TupleLike U, typename I = std::make_index_sequence<std::tuple_size_v<T>>>
    struct TupleLikeCommonType;

    template <class T, class U, size_t... I>
    concept TupleLikeCommonTypeHelper = requires {
        typename std::tuple<std::common_type_t<std::tuple_element_t<I, T>, std::tuple_element_t<I, U>>...>;
    };

    RETROLIB_EXPORT template <class T, class U, size_t... I>
        requires TupleLikeCommonTypeHelper<T, U, I...>
    struct TupleLikeCommonType<T, U, std::index_sequence<I...>> {
        using Type = std::tuple<std::common_type_t<std::tuple_element_t<I, T>, std::tuple_element_t<I, U>>...>;
    };
} // namespace retro