/**
 * @file Get.h
 * @brief Functional chain binding for getting an optional's value.
 *
 * @author Retro & Chill
 * https://github.com/retroandchill
 */
#pragma once

#include "RetroLib/Functional/ExtensionMethods.h"
#include "RetroLib/Optionals/OptionalOperations.h"

#ifndef RETROLIB_EXPORT
#define RETROLIB_EXPORT
#endif

namespace Retro::Optionals {

    struct FValueInvoker {

        template <OptionalType O>
        constexpr decltype(auto) operator()(O &&Optional) const {
            RETROLIB_ASSERT(HasValue(std::forward<O>(Optional)));
            return Get(std::forward<O>(Optional));
        }
    };

    /**
     * Get the value of passed/piped optional.
     */
    RETROLIB_EXPORT constexpr auto Value = ExtensionMethod<FValueInvoker{}>();

} // namespace retro::optionals
