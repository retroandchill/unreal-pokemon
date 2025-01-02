/**
 * @file CacheLast.h
 * @brief Caching container used for holding onto temporaries returned from a range-pipe.
 *
 * @author Retro & Chill
 * https://github.com/retroandchill
 */
#pragma once

#include "RetroLib/Ranges/FeatureBridge.h"
#include "RetroLib/Ranges/RangeBasics.h"
#include "RetroLib/Utils/NonPropagatingCache.h"

#ifndef RETROLIB_EXPORT
#define RETROLIB_EXPORT
#endif

namespace Retro::Ranges {

    /**
     * @class TCacheLastView
     * @brief A custom view that caches the last element accessed for optimization purposes.
     *
     * @tparam R The type of the underlying range.
     *
     * This class provides a view over the specified range and ensures that the most recently
     * accessed value is cached locally for efficient repeated access. It supports ranged-based
     * iteration and other range utilities.
     */
    RETROLIB_EXPORT template <std::ranges::input_range R>
        requires std::ranges::view<R> &&
                 std::constructible_from<std::ranges::range_value_t<R>, std::ranges::range_reference_t<R>>
    class TCacheLastView : std::ranges::view_interface<TCacheLastView<R>> {

        class FIterator;

        class FSentinel {
          public:
            constexpr FSentinel() = default;
            constexpr explicit FSentinel(std::ranges::sentinel_t<R> Last) : Last(std::move(Last)) {
            }

            constexpr std::ranges::range_difference_t<R> operator-(const FIterator &Other) const
                requires std::sized_sentinel_for<std::ranges::sentinel_t<R>, std::ranges::sentinel_t<R>>
            {
                return Last - Other.it;
            }

          private:
            friend FIterator;
            std::ranges::sentinel_t<R> Last;
        };

        class FIterator {

          public:
            using value_type = std::ranges::range_value_t<R>;
            using single_pass = std::true_type;
            using difference_type = std::ranges::range_difference_t<R>;

            constexpr FIterator() = default;
            constexpr FIterator(TCacheLastView *Parent, std::ranges::iterator_t<R> It)
                : Parent(Parent), It(std::move(It)) {
            }

            constexpr std::ranges::range_value_t<R> &&operator*() const {
                if (Parent->Dirty) {
                    Parent->Update(*It);
                    Parent->Dirty = false;
                }

                return std::move(*Parent->Cache);
            }

            constexpr bool operator==(const FIterator &Other) const
                requires std::ranges::forward_range<R>
            {
                return It == Other.It;
            }

            constexpr bool operator==(const FSentinel &Other) const {
                return It == Other.Last;
            }

            constexpr FIterator &operator++() {
                ++It;
                Parent->Dirty = true;
                return *this;
            }

            constexpr auto operator++(int) {
                if constexpr (std::ranges::forward_range<R>) {
                    auto Tmp = *this;
                    ++*this;
                    return Tmp;
                } else {
                    ++*this;
                }
            }

            constexpr difference_type operator-(const FIterator &Other) const
                requires std::sized_sentinel_for<std::ranges::iterator_t<R>, std::ranges::iterator_t<R>>
            {
                return It - Other.It;
            }

            constexpr difference_type operator-(const FSentinel &Other) const
                requires std::sized_sentinel_for<std::ranges::sentinel_t<R>, std::ranges::iterator_t<R>>
            {
                return It - Other.Last;
            }

          private:
            TCacheLastView *Parent;
            std::ranges::iterator_t<R> It;
        };

      public:
        /**
         * @brief Default constructor for the CacheLastView class.
         *
         * This constructor is declared as constexpr, allowing it
         * to be evaluated at compile-time. It performs no initialization
         * and is marked as default.
         */
        constexpr TCacheLastView() = default;

        /**
         * @brief Constructs a CacheLastView object with the specified range.
         *
         * This constructor is declared as constexpr and explicit, enabling compile-time
         * evaluation when applicable. It initializes the CacheLastView with the given range,
         * transferring ownership of the range using std::move to avoid unnecessary copies.
         *
         * @param Range The range to be cached, transferred using move semantics.
         */
        constexpr explicit TCacheLastView(R Range) : Range(std::move(Range)) {
        }

        /**
         * @brief Provides an iterator to the beginning of the range.
         *
         * Marks the range as dirty, indicating it has been modified,
         * and returns an iterator pointing to the first element of the range.
         *
         * @return An iterator to the beginning of the range.
         */
        FIterator begin() {
            Dirty = true;
            return FIterator(this, std::ranges::begin(Range));
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
        FIterator end() {
            if constexpr (std::ranges::common_range<R>) {
                return FIterator(this, std::ranges::end(Range));
            } else {
                return FSentinel(std::ranges::end(Range));
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
        constexpr auto size() const
            requires std::ranges::sized_range<R>
        {
            return std::ranges::size(Range);
        }

      private:
        constexpr void Update(std::ranges::range_reference_t<R> &&value)
            requires std::assignable_from<std::ranges::range_value_t<R> &, std::ranges::range_value_t<R>>
        {
            if (!Cache.has_value()) {
                Cache.emplace(static_cast<std::ranges::range_reference_t<R> &&>(value));
            } else {
                *Cache = static_cast<std::ranges::range_reference_t<R> &&>(value);
            }
        }

        constexpr auto Update(std::ranges::range_reference_t<R> &&value)
            requires(!std::assignable_from<std::ranges::range_value_t<R> &, std::ranges::range_value_t<R>>)
        {
            Cache.emplace(static_cast<std::ranges::range_reference_t<R> &&>(value));
        }

        R Range;
        bool Dirty = true;
        TNonPropagatingCache<std::ranges::range_value_t<R>> Cache;
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
    TCacheLastView(R &&) -> TCacheLastView<std::ranges::views::all_t<R>>;

    namespace Views {
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
             * @param Range A forwarding reference to the range to be processed.
             * @return A CacheLastView constructed from the provided range.
             */
            template <std::ranges::input_range R>
                requires std::ranges::viewable_range<R> &&
                         std::constructible_from<std::ranges::range_value_t<R>, std::ranges::range_reference_t<R>>
            constexpr auto operator()(R &&Range) const {
                return TCacheLastView(std::ranges::views::all(std::forward<R>(Range)));
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
        RETROLIB_EXPORT constexpr auto CacheLast = ExtensionMethod<CacheLastInvoker{}>();
    } // namespace views
} // namespace retro::ranges