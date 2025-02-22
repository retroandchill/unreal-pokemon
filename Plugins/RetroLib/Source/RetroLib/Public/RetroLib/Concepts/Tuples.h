/**
 * @file Tuples.h
 * @brief Concepts pertaining to tuples.
 *
 * @author Retro & Chill
 * https://github.com/retroandchill
 */
#pragma once

#include "RetroLib/FunctionTraits.h"
#include "RetroLib/Utils/ForwardLike.h"

#if !RETROLIB_WITH_MODULES
#include "RetroLib/RetroLibMacros.h"
#include <optional>
#include <tuple>
#include <type_traits>
#endif

#ifndef RETROLIB_EXPORT
#define RETROLIB_EXPORT
#endif

namespace Retro {

    /**
     * The type of forwarded tuple element. Normally it's the same as using `forward_like`, except when the tuple
     * element is an l-value reference, where the reference is retained.
     *
     * @tparam T The tuple index
     * @tparam I The index to look up
     */
    RETROLIB_EXPORT template <typename T, size_t I>
    using TForwardedTupleType = std::conditional_t<std::is_lvalue_reference_v<std::tuple_element_t<I, std::decay_t<T>>>,
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
        // TODO: This isn't ideal but for some reason ADL does not work when compiling modules
        typename std::tuple_element_t<I, T>;
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
    consteval bool AllHasTupleElement(std::index_sequence<I...>) {
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
    } && AllHasTupleElement<T>(std::make_index_sequence<std::tuple_size_v<T>>{});

    /**
     * Checks if the given args can be applied to the given functor type.
     *
     * @tparam F The functional type
     * @tparam T The tuple type
     * @param I The indicies that are to be applied as arguments
     */
    template <typename F, typename T, size_t... I>
    concept CanApplyArgs = HasFunctionCallOperator<std::decay_t<F>> && TupleLike<std::decay_t<T>> &&
                           std::invocable<F, TForwardedTupleType<T, I>...>;

    /**
     * Determines if the template arguments can be applied with an index sequence.
     *
     * @param std::index_sequence<I...> An index sequence of integers used to check
     *                                  applicability of template arguments.
     * @return bool Returns true if the template arguments can be successfully applied with
     *              the given index sequence, otherwise false.
     */
    template <typename F, typename T, size_t... I>
    consteval bool CanApplyIndexSequence(std::index_sequence<I...>) {
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
                       CanApplyIndexSequence<F, T>(std::make_index_sequence<std::tuple_size_v<std::decay_t<T>>>{});

    /**
     * Checks if the given args when applied to the given functor type is noexcept.
     *
     * @tparam F The functional type
     * @tparam T The tuple type
     * @param I The indicies that are to be applied as arguments
     */
    template <typename F, typename T, size_t... I>
    concept NoThrowApplicableArgs = HasFunctionCallOperator<std::decay_t<F>> && TupleLike<std::decay_t<T>> &&
                                    std::is_nothrow_invocable_v<F, TForwardedTupleType<T, I>...>;

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
    consteval bool IsNoThrowApplicable(std::index_sequence<I...>) {
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
        IsNoThrowApplicable<F, T>(std::make_index_sequence<std::tuple_size_v<std::decay_t<T>>>{});

    template <TupleLike T, typename A, size_t... I>
    consteval bool HasTypeImpl(std::index_sequence<I...>) {
        return (std::same_as<A, std::tuple_element_t<I, T>> || ...);
    }

    template <typename T, typename A>
    concept HasType = TupleLike<T> && HasTypeImpl<T, A>(std::make_index_sequence<std::tuple_size_v<T>>{});

    template <TupleLike T, TupleLike U, size_t... I>
    consteval bool HasOverlappingTypesImpl(std::index_sequence<I...>) {
        return (HasType<U, std::tuple_element_t<I, T>> || ...);
    }

    template <typename T, typename U>
    concept HasOverlappingTypes =
        TupleLike<T> && TupleLike<U> && HasOverlappingTypesImpl<T, U>(std::make_index_sequence<std::tuple_size_v<T>>{});

    template <TupleLike T, typename A, size_t... I>
    consteval std::optional<size_t> GetTypeIndexImpl(std::index_sequence<I...>) {
        return (std::same_as<A, std::tuple_element_t<I, T>> || ...);
    }

    template <size_t I, typename A, typename T>
    consteval std::optional<size_t> GetIndexOfType() {
        if constexpr (I >= std::tuple_size_v<T>) {
            return std::nullopt;
        } else if constexpr (std::same_as<A, std::tuple_element_t<I, T>>) {
            return I;
        } else {
            return GetIndexOfType<I + 1, A, T>();
        }
    }

    template <typename A, TupleLike T>
    constexpr auto IndexInTuple = GetIndexOfType<0, A, T>();

    template <TupleLike T, TupleLike U, size_t... I>
    consteval auto GetMatchingTupleIndexesImpl(std::index_sequence<I...>) {
        return std::array{IndexInTuple<std::tuple_element_t<I, U>, T>...};
    }

    template <TupleLike T, TupleLike U>
    constexpr auto MatchingTupleIndexes =
        GetMatchingTupleIndexesImpl<T, U>(std::make_index_sequence<std::tuple_size_v<U>>{});
} // namespace Retro
