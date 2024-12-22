/**
 * @file Get.h
 * @brief Functional chain binding for getting an optional's value.
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

    struct ValueInvoker {

        template <OptionalType O>
        constexpr decltype(auto) operator()(O &&optional) const {
            RETROLIB_ASSERT(has_value(std::forward<O>(optional)));
            return get(std::forward<O>(optional));
        }
    };

    /**
     * Get the value of passed/piped optional.
     */
    RETROLIB_EXPORT constexpr auto value = extension_method<ValueInvoker{}>();

} // namespace retro::optionals
