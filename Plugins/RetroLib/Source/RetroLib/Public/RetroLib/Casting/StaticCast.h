/**
 * @file StaticCast.h
 * @brief Static conversion functor between types.
 *
 * @author Retro & Chill
 * https://github.com/retroandchill
 */
#pragma once

#include "RetroLib/Concepts/Inheritance.h"

#if !RETROLIB_WITH_MODULES
#include <type_traits>
#endif

#ifndef RETROLIB_EXPORT
#define RETROLIB_EXPORT
#endif

namespace Retro {

    /**
     * Checks if you can cast from T to U.
     *
     * @tparam T The type of the source parameter
     * @tparam U The target type
     */
    RETROLIB_EXPORT template <typename T, typename U>
    concept CanStaticCast = requires(T &&Input) {
        { static_cast<U>(std::forward<T>(Input)) } -> std::same_as<U>;
    };

    /**
     * @class TStaticCastFunction
     *
     * @brief A generic conversion functor for types convertible to a specified type T.
     *
     * This struct provides a call operator that performs type conversion on
     * its argument to a predefined target type T. The conversion is only enabled
     * if the argument type is convertible to the target type T.
     *
     * @tparam T The target type to which the given arguments will be converted.
     *
     * @note The conversion is done using static_cast, which requires the types
     *       to be implicitly convertible.
     */
    template <typename T>
    struct TStaticCastFunction {
        /**
         * @brief Invokes the conversion of the provided argument to the specified type T.
         *
         * This function call operator allows for the conversion of an argument
         * of type U to the target type T using static_cast. It utilizes perfect forwarding
         * to preserve the value category of the argument passed to it.
         *
         * @tparam U The type of the argument to be converted.
         *
         * @param Args The argument to be converted to type T.
         *
         * @return The result of converting the argument to type T.
         *
         * @note The conversion is done using static_cast, which requires the types
         *       to be implicitly convertible. This operator is defined as constexpr,
         *       allowing it to be evaluated at compile time if all components involved
         *       support constant evaluation.
         */
        template <typename U>
            requires CanStaticCast<U, T>
        constexpr decltype(auto) operator()(U &&Args) const {
            return static_cast<T>(std::forward<U>(Args));
        }
    };

    /**
     * @brief An instance of the Convert functor template specialized for a specific type T.
     *
     * This constant expression represents a pre-instantiated functor object of the Convert
     * template that allows the conversion of data to the type T. This instance can be used
     * directly to perform conversions, leveraging the predefined conversion logic encapsulated
     * within the Convert template.
     *
     * @tparam T The type to which the functor converts arguments.
     */
    RETROLIB_EXPORT template <typename T>
    constexpr TStaticCastFunction<T> StaticCast;
} // namespace retro