/**
 * @file PtrOrNull.h
 * @brief Get either a raw pointer value or null.
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
#include <RetroLib/TypeTraits.h>

namespace Retro::Optionals {

    struct PtrOrNullInvoker {

        template <OptionalType O>
            requires(std::is_lvalue_reference_v<O> || OptionalReference<O>) &&
                    (!SpecializationOf<ValueType<O>, std::reference_wrapper>)
        constexpr std::add_pointer_t<ValueType<O>> operator()(O &&Optional) const {
            return HasValue(std::forward<O>(Optional)) ? &Get(std::forward<O>(Optional)) : nullptr;
        }

        template <template <typename...> typename O, typename T>
            requires OptionalType<O<std::reference_wrapper<T>>>
        constexpr std::add_pointer_t<T> operator()(const O<std::reference_wrapper<T>> &Optional) const {
            return HasValue(Optional) ? &Get(Optional).get() : nullptr;
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
    RETROLIB_EXPORT constexpr auto PtrOrNull = ExtensionMethod<PtrOrNullInvoker{}>();

} // namespace retro::optionals
