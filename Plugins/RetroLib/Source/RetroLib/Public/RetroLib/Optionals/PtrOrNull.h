/**
 * @file PtrOrNull.h
 * @brief Get either a raw pointer value or null.
 *
 * @author Retro & Chill
 * https://github.com/retroandchill
 */
#pragma once

#include "RetroLib/Concepts/Pointers.h"
#include "RetroLib/Functional/ExtensionMethods.h"
#include "RetroLib/Optionals/OptionalOperations.h"
#include "RetroLib/TypeTraits.h"

#ifndef RETROLIB_EXPORT
#define RETROLIB_EXPORT
#endif

namespace Retro::Optionals {

    struct FPtrOrNullInvoker {

        template <OptionalType O>
            requires(!SpecializationOf<TValueType<O>, std::reference_wrapper>)
        constexpr auto operator()(O &&Optional) const {
            return HasValue(std::forward<O>(Optional)) ? GetPtrFromRef(Get(std::forward<O>(Optional))) : nullptr;
        }

        template <template <typename...> typename O, typename T>
            requires OptionalType<O<std::reference_wrapper<T>>>
        constexpr auto operator()(const O<std::reference_wrapper<T>> &Optional) const {
            return HasValue(Optional) ? GetPtrFromRef(Get(Optional).get()) : nullptr;
        }
    };

    /**
     * @brief Transforms an optional object into a pointer or a null pointer.
     *
     * The `ptr_or_null` variable acts as an extension method, enabling operations
     * on `std::optional` values to efficiently retrieve a pointer to the contained
     * value if it exists or a `nullptr` if the optional is empty.
     *
     * @details
     * Utilizing this in pipelines helps seamlessly transition from optional objects
     * to pointers, simplifying integration with pointer-based APIs or operations
     * that accept raw or smart pointers.
     */
    RETROLIB_EXPORT constexpr auto PtrOrNull = ExtensionMethod<FPtrOrNullInvoker{}>();

} // namespace Retro::Optionals
