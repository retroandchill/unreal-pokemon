/**
 * @file NonPropagatingCache.h
 * @brief Basic non-propagating cache type
 *
 * @author Retro & Chill
 * https://github.com/retroandchill
 */

#pragma once

#if !RETROLIB_WITH_MODULES
#include "RetroLib/Optionals/Optional.h"
#endif

#ifndef RETROLIB_EXPORT
#define RETROLIB_EXPORT
#endif

namespace Retro {
    /**
     * @class NonPropagatingCache
     * @brief A specialized cache class based on the Optional template.
     *
     * This class is designed to prevent propagation of its state when copied or moved.
     * Assignments of objects of this type explicitly reset the cache state.
     *
     * @tparam T The type of the value stored in the cache.
     */
    template <typename T, typename U = void, bool Enable = true>
    class NonPropagatingCache : public Optional<T> {
      public:
        /**
         * @brief Constructs a NonPropagatingCache object with default initialization.
         *
         * This constructor is a constexpr and performs default initialization for the
         * NonPropagatingCache object.
         *
         * @return A default-initialized NonPropagatingCache instance.
         */
        constexpr NonPropagatingCache() = default;

        /**
         * @brief Constructs a NonPropagatingCache object with an explicitly nullopt state.
         *
         * This constructor initializes a NonPropagatingCache object to a nullopt state.
         * It is marked as noexcept and does not allow implicit conversions.
         *
         * @param std::nullopt_t A tag type used to initialize the cache with a nullopt state.
         * @return An instance of NonPropagatingCache initialized with nullopt.
         */
        constexpr explicit(false) NonPropagatingCache(std::nullopt_t) noexcept {
        }

        /**
         * @brief Constructs a NonPropagatingCache object as a copy of another instance.
         *
         * This constructor is a constexpr and performs a copy construction for the
         * NonPropagatingCache object. It is marked as noexcept.
         *
         * @param other The NonPropagatingCache instance to copy from.
         * @return A copy of the provided NonPropagatingCache instance.
         */
        constexpr NonPropagatingCache(const NonPropagatingCache &) noexcept : Optional<T>() {
        }

        /**
         * @brief Moves an existing NonPropagatingCache object into a new instance.
         *
         * This move constructor safely transfers ownership from the given rvalue
         * NonPropagatingCache to a new object without propagating cache data.
         *
         * @param other The rvalue NonPropagatingCache instance to be moved.
         * @return A new NonPropagatingCache instance initialized from the given rvalue.
         */
        constexpr NonPropagatingCache(NonPropagatingCache &&) noexcept : Optional<T>() {
        }

        /**
         * @brief Default destructor for the NonPropagatingCache class.
         *
         * Automatically cleans up resources used by a NonPropagatingCache object
         * without requiring explicit custom cleanup operations.
         */
        ~NonPropagatingCache() = default;

        /**
         * @brief Copy assignment operator for the NonPropagatingCache class.
         *
         * Assigns the contents of the provided NonPropagatingCache instance to the current instance.
         * Resets the current instance and copies the state from the provided instance.
         *
         * @param other The NonPropagatingCache instance to copy from.
         * @return A reference to the current NonPropagatingCache instance after assignment.
         */
        constexpr NonPropagatingCache &operator=(const NonPropagatingCache &) noexcept {
            Optional<T>::reset();
            return *this;
        }

        /**
         * @brief Move assignment operator for NonPropagatingCache.
         *
         * Transfers the state of another NonPropagatingCache object to this instance
         * through move semantics. Resets the source NonPropagatingCache and ensures
         * the current object is also reset.
         *
         * @param Other The NonPropagatingCache object to move from.
         * @return A reference to the current NonPropagatingCache object after the move.
         */
        constexpr NonPropagatingCache &operator=(NonPropagatingCache &&Other) noexcept {
            Other.Optional<T>::Reset();
            Optional<T>::Reset();
            return *this;
        }

        using Optional<T>::operator=;

        /**
         * @brief Inserts a new value by dereferencing the given iterator and returns a reference to the emplaced value.
         *
         * This method dereferences the provided iterator and uses the resulting value
         * to construct or insert a new value, returning a reference to it.
         *
         * @param i The iterator whose dereferenced value will be used to emplace a new value.
         * @return A reference to the newly emplaced value of type T.
         */
        template <typename I>
        constexpr T &EmplaceDeref(const I &i) {
            return Optional<T>::emplace(*i);
        }
    };

    /**
     * @brief Represents a specialized version of NonPropagatingCache with specific parameterization.
     *
     * This specialization is used when the third template parameter is explicitly set to false.
     * It provides behavior tailored to scenarios where propagation is disabled. This struct
     * does not support propagation-based caching mechanisms.
     *
     * @tparam T First template parameter, representing the key or identifier type for the cache.
     * @tparam U Second template parameter, representing the value or data type stored in the cache.
     */
    template <typename T, typename U>
    struct NonPropagatingCache<T, U, false> {};
} // namespace retro