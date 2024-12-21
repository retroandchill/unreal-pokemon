/**
 * @file IsSet.h
 * @brief Pipe to check to see if an optional has a value
 *
 * @author Retro & Chill
 * https://github.com/retroandchill
 */
#pragma once

#if !RETROLIB_WITH_MODULES
#include "RetroLib/Functional/ExtensionMethods.h"
#include "RetroLib/Optionals/OptionalOperations.h"
#endif

#ifndef RETROLIB_EXPORT
#define RETROLIB_EXPORT
#endif

namespace retro::optionals {

    struct IsSetInvoker {
        template <OptionalType O>
        constexpr bool operator()(O &&optional) const {
            return has_value(std::forward<O>(optional));
        }
    };

    /**
     * @brief Represents a constexpr that defines an extension method for invoking a set-check mechanism.
     *
     * @details This construct creates a compile-time constant named `is_set` using
     * the `extension_method` function and associates it with the `IsSetInvoker` object.
     * It is intended to be used as an invocation mechanism to check if a particular
     * set condition is satisfied or not.
     *
     * The `IsSetInvoker` serves as the custom logic handler for the set-check functionality.
     * The functionality provided by this constexpr is designed for use in conjunction
     * with extension methods in environments where enhanced or dynamic behaviors are needed
     * based on compile-time extensions.
     */
    RETROLIB_EXPORT constexpr auto is_set = extension_method<IsSetInvoker{}>();

} // namespace retro::optionals
