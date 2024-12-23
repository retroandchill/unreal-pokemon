/**
 * @file IfPresentOrElse.h
 * @brief Perform an action of the value if its present, otherwise do something else.
 *
 * @author Retro & Chill
 * https://github.com/retroandchill
 */
#pragma once

#if !RETROLIB_WITH_MODULES
#include "RetroLib/Functional/CreateBinding.h"
#include "RetroLib/Functional/FunctionalClosure.h"
#include "RetroLib/Optionals/OptionalOperations.h"
#endif

#ifndef RETROLIB_EXPORT
#define RETROLIB_EXPORT
#endif

namespace retro::optionals {
    struct IfPresentOrElseInvoker {
        /**
         * Provides a callable operator to conditionally invoke functions based on the presence or absence of a value.
         *
         * @param optional An object to check for a value using `has_value`. Typically, this could be an optional-like object.
         * @param present A callable function that will be invoked if `optional` contains a value. It is called with the value extracted from `optional`.
         * @param absent A callable function that will be invoked if `optional` does not contain a value. It takes no arguments.
         */
        template <OptionalType O, typename F, typename G>
            requires std::invocable<F, CommonReference<O>> && std::invocable<G>
        constexpr void operator()(O&& optional, F&& present, G&& absent) const {
            if (has_value(std::forward<O>(optional))) {
                std::invoke(std::forward<F>(present), get(std::forward<O>(optional)));
            } else {
                std::invoke(std::forward<G>(absent));
            }
        }
    };

    /**
     * @brief A constexpr variable used to define the `if_present_or_else` extension method.
     *
     * This utility is part of a functional pipeline supporting optional handling, enabling
     * the chaining of operations on `std::optional` objects. It applies one of two functions
     * based on whether the `std::optional` value is present or not.
     *
     * When chained, it invokes the first provided callable if the `std::optional` contains
     * a value, and invokes the second callable if the `std::optional` is empty. This helps
     * simplify and streamline decision-making logic for `std::optional` values within
     * functional code entities.
     */
    RETROLIB_EXPORT constexpr auto if_present_or_else = extension_method<IfPresentOrElseInvoker{}>;
}