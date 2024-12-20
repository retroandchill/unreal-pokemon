/**
 * @file CacheLast.h
 * @brief Caching container used for holding onto temporaries returned from a range-pipe.
 *
 * @author Retro & Chill
 * https://github.com/retroandchill
 */
#pragma once

#if !RETROLIB_WITH_MODULES
#include "RetroLib/Ranges/RangeBasics.h"
#include "RetroLib/Ranges/FeatureBridge.h"
#include "RetroLib/Utils/NonPropagatingCache.h"
#endif

#ifndef RETROLIB_EXPORT
#define RETROLIB_EXPORT
#endif

namespace retro::ranges {

    /**
     * @class CacheLastView
     * @brief A custom view that caches the last element accessed for optimization purposes.
     *
     * @tparam R The type of the underlying range.
     *
     * This class provides a view over the specified range and ensures that the most recently
     * accessed value is cached locally for efficient repeated access. It supports ranged-based
     * iteration and other range utilities.
     */
    RETROLIB_EXPORT template <std::ranges::input_range R>
        requires std::ranges::viewable_range<R> && std::constructible_from<std::ranges::range_value_t<R>, std::ranges::range_reference_t<R>>
    class CacheLastView : std::ranges::view_interface<CacheLastView<R>> {

        class Iterator;

        class Sentinel {
        public:
            constexpr Sentinel() = default;
            constexpr explicit Sentinel(std::ranges::sentinel_t<R> last) : last(std::move(last)) {}

            constexpr std::ranges::range_difference_t<R> operator-(const Iterator& other) const requires std::sized_sentinel_for<std::ranges::sentinel_t<R>, std::ranges::sentinel_t<R>> {
                return last - other.it;
            }

        private:
            friend Iterator;
            std::ranges::sentinel_t<R> last;
        };

        class Iterator {

        public:
            using value_type = std::ranges::range_value_t<R>;
            using single_pass = std::true_type;
            using difference_type = std::ranges::range_difference_t<R>;

            constexpr Iterator() = default;
            constexpr Iterator(CacheLastView* parent, std::ranges::iterator_t<R> it) : parent(parent), it(std::move(it)) {}

            constexpr std::ranges::range_value_t<R>&& operator*() const {
                if (parent->dirty) {
                    parent->update(*it);
                    parent->dirty = false;
                }

                return std::move(*parent->cache);
            }

            constexpr bool operator==(const Iterator& other) const requires std::ranges::forward_range<R> {
                return it == other.it;
            }

            constexpr bool operator==(const Sentinel& other) const {
                return it == other.last;
            }

            constexpr Iterator& operator++() {
                ++it;
                parent->dirty = true;
                return *this;
            }

            constexpr auto operator++(int) {
                if constexpr (std::ranges::forward_range<R>) {
                    auto tmp = *this;
                    ++*this;
                    return tmp;
                } else {
                    ++*this;
                }
            }

            constexpr difference_type operator-(const Iterator& other) const requires std::sized_sentinel_for<std::ranges::iterator_t<R>, std::ranges::iterator_t<R>> {
                return it - other.it;
            }

            constexpr difference_type operator-(const Sentinel& other) const requires std::sized_sentinel_for<std::ranges::sentinel_t<R>, std::ranges::iterator_t<R>> {
                return it - other.last;
            }

        private:
            CacheLastView* parent;
            std::ranges::iterator_t<R> it;
        };

    public:
        /**
         * @brief Default constructor for the CacheLastView class.
         *
         * This constructor is declared as constexpr, allowing it
         * to be evaluated at compile-time. It performs no initialization
         * and is marked as default.
         */
        constexpr CacheLastView() = default;

        /**
         * @brief Constructs a CacheLastView object with the specified range.
         *
         * This constructor is declared as constexpr and explicit, enabling compile-time
         * evaluation when applicable. It initializes the CacheLastView with the given range,
         * transferring ownership of the range using std::move to avoid unnecessary copies.
         *
         * @param range The range to be cached, transferred using move semantics.
         */
        constexpr explicit CacheLastView(R range) : range(std::move(range)) {}


        /**
         * @brief Provides an iterator to the beginning of the range.
         *
         * Marks the range as dirty, indicating it has been modified,
         * and returns an iterator pointing to the first element of the range.
         *
         * @return An iterator to the beginning of the range.
         */
        Iterator begin() {
            dirty = true;
            return Iterator(this, std::ranges::begin(range));
        }

        /**
         * @brief Returns an iterator or sentinel representing the end of the range.
         *
         * If the range satisfies the common_range concept, returns an Iterator object
         * that represents the end of the range. Otherwise, returns a Sentinel object
         * that marks the end of the range.
         *
         * @return An Iterator or Sentinel representing the end of the range.
         */
        Iterator end()  {
            if constexpr (std::ranges::common_range<R>) {
                return Iterator(this, std::ranges::end(range));
            } else {
                return Sentinel(std::ranges::end(range));
            }
        }

        /**
         * @brief Returns the size of the given range.
         *
         * This method is available only if the range satisfies the `std::ranges::sized_range` constraint.
         * It computes the size of the range using `std::ranges::size`.
         *
         * @return The size of the range.
         */
        constexpr auto size() const requires std::ranges::sized_range<R> {
            return std::ranges::size(range);
        }

    private:
        constexpr void update(std::ranges::range_reference_t<R>&& value) requires std::assignable_from<std::ranges::range_value_t<R>&, std::ranges::range_value_t<R>> {
            if (!cache.has_value()) {
                cache.emplace(static_cast<std::ranges::range_reference_t<R>&&>(value));
            } else {
                *cache = static_cast<std::ranges::range_reference_t<R>&&>(value);
            }
        }

        constexpr auto update(std::ranges::range_reference_t<R>&& value) requires (!std::assignable_from<std::ranges::range_value_t<R>&, std::ranges::range_value_t<R>>) {
            cache.emplace(static_cast<std::ranges::range_reference_t<R>&&>(value));
        }

        R range;
        bool dirty = true;
        NonPropagatingCache<std::ranges::range_value_t<R>> cache;
    };

    /**
     * Creates a CacheLastView object from the given range, wrapping it in a view
     * that caches the last element for efficient access.
     *
     * @param range The input range to be wrapped and processed.
     * @return A CacheLastView object that provides a view of the input range
     *         with a cached last element.
     */
    template <std::ranges::input_range R>
    CacheLastView(R&&) -> CacheLastView<std::ranges::views::all_t<R>>;

    namespace views {
        /**
         * @brief Functor for invoking a cache-last view transformation on an input range.
         *
         * This functor is a callable structure that takes an input range `R` and
         * applies the `CacheLastView` transformation to it. The transformation ensures
         * that the input range is viewable and constructible from its range reference type.
         *
         * The operator() is constexpr and leverages composition with `std::ranges::views::all`
         * to adapt the input range into a suitable form before applying the transformation.
         *
         * @tparam R The type of the input range. Must satisfy the constraints of being an
         *           input range, a viewable range, and have elements constructible from
         *           the range's reference type.
         */
        struct CacheLastInvoker {

            /**
             * @brief Function call operator to create a CacheLastView from a given range.
             *
             * This operator takes a forwarding reference to a range and constructs
             * a CacheLastView by applying the `std::ranges::views::all` view to the range.
             *
             * @param range A forwarding reference to the range to be processed.
             * @return A CacheLastView constructed from the provided range.
             */
            template <std::ranges::input_range R>
                requires std::ranges::viewable_range<R> && std::constructible_from<std::ranges::range_value_t<R>, std::ranges::range_reference_t<R>>
            constexpr auto operator()(R&& range) const {
                return CacheLastView(std::ranges::views::all(std::forward<R>(range)));
            }
        };

        /**
         * @brief Defines a constexpr instantiation for the invocation of the CacheLast view.
         *
         * This variable template is used to create a view adapter that caches
         * the last element of a range. It is implemented as an extension method
         * utilizing the functionality provided by the CacheLastInvoker struct.
         *
         * The cache_last adapter is designed to be composed with other range
         * views, providing efficient access to the last element without
         * recalculating it at every access, even for non-bidirectional ranges.
         */
        RETROLIB_EXPORT constexpr auto cache_last = extension_method<CacheLastInvoker{}>();
    }
}