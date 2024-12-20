/**
 * @file OptionalIterator.h
 * @brief Iterator support for optional types
 *
 * @author Retro & Chill
 * https://github.com/retroandchill
 */
#pragma once

#if !RETROLIB_WITH_MODULES
#include "RetroLib/Concepts/Operators.h"
#include "RetroLib/Concepts/ParameterPacks.h"
#include "RetroLib/Optionals/OptionalOperations.h"
#include "RetroLib/RetroLibMacros.h"

#include <utility>
#endif

#ifndef RETROLIB_EXPORT
#define RETROLIB_EXPORT
#endif

namespace retro::optionals {

    /**
     * @struct OptionalSentinel
     *
     * Sentinel value for iterating through an optional value. Represents having reached the end of iteration.
     */
    RETROLIB_EXPORT struct OptionalSentinel {};

    /**
     * @struct OptionalIterator
     *
     * Iterator type for iterating through an optional, as if it were a range. If the optional is empty, the range is
     * considered to be empty.
     *
     * @tparam O The type of optional that is contained.
     */
    RETROLIB_EXPORT template <OptionalType O>
    struct OptionalIterator {
        using value_type = ValueType<O>;
        using difference_type = std::ptrdiff_t;

        /**
         * @brief Default constructor for `OptionalIterator`.
         *
         * Constructs an `OptionalIterator` instance with a default state. This
         * ensures that the object is initialized appropriately without requiring
         * any specific argument for construction.
         *
         * @return A new `OptionalIterator` object with a default state.
         */
        constexpr OptionalIterator() = default;

        /**
         * @brief Constructs an `OptionalIterator` with a reference to a value.
         *
         * Creates an instance of `OptionalIterator` by explicitly passing a reference
         * to an object to manage. This ensures the iterator has a defined value it can
         * access and operate on.
         *
         * @param value A reference to the object this iterator will manage.
         */
        constexpr explicit OptionalIterator(O &value) : value(&value) {
        }

        /**
         * @brief Dereference operator for accessing the stored value.
         *
         * Provides access to the value stored within the object. This operator
         * asserts that the stored pointer is not null and contains a valid value
         * before dereferencing it.
         *
         * @return A constant reference or value of the dereferenced stored object.
         */
        constexpr decltype(auto) operator*() const {
            RETROLIB_ASSERT(value != nullptr && has_value(*value));
            return **value;
        }

        /**
         * @brief Equality comparison operator for `OptionalIterator`.
         *
         * Compares this `OptionalIterator` instance with another to determine if they
         * are equal based on their internal values.
         *
         * @param other The `OptionalIterator` instance to compare against.
         * @return `true` if both `OptionalIterator` instances have equal values,
         *         otherwise `false`.
         */
        constexpr bool operator==(const OptionalIterator &other) const {
            return value == other.value;
        }

        /**
         * @brief Equality operator for `OptionalSentinel`.
         *
         * Compares the current `OptionalSentinel` with another instance to check
         * if they are considered equal. Equality is determined based on the value
         * state or if the value does not satisfy the expected condition.
         *
         * @return `true` if the instances are considered equal, `false` otherwise.
         */
        constexpr bool operator==(const OptionalSentinel &) const {
            return value == nullptr || !has_value(*value);
        }

        /**
         * @brief Increment operator for `OptionalIterator`.
         *
         * Advances the state of the `OptionalIterator` by nullifying its value. Ensures
         * internal consistency by asserting that the current value is not null before
         * performing the operation.
         *
         * @return A reference to the updated `OptionalIterator` object.
         */
        constexpr OptionalIterator &operator++() {
            RETROLIB_ASSERT(value != nullptr);
            value = nullptr;
            return *this;
        }

        /**
         * @brief Post-increment operator for `OptionalIterator`.
         *
         * Creates a copy of the current `OptionalIterator` instance, increments the
         * current instance, and returns the copy. This operator allows the iterator
         * to be incremented in the post-increment style.
         *
         * @return A copy of the `OptionalIterator` instance before it was incremented.
         */
        constexpr OptionalIterator operator++(int) {
            OptionalIterator copy = *this;
            ++*this;
            return copy;
        }

      private:
        O *value = nullptr;
    };

    /**
     * Constructs an OptionalIterator by deducing the template type from the provided object.
     *
     * @param O The object from which the OptionalIterator's template type is deduced.
     *          The type is determined by removing the reference from the provided object.
     * @return An OptionalIterator instance with the deduced template type.
     */
    RETROLIB_EXPORT template <OptionalType O>
    OptionalIterator(O &&) -> OptionalIterator<std::remove_reference_t<O>>;

} // namespace retro::optionals