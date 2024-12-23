/**
 * @file Elements.h
 * @brief Gets the individual elements of a tuple-like structure.
 *
 * @author Retro & Chill
 * https://github.com/retroandchill
 */
#pragma once

#if !RETROLIB_WITH_MODULES
#include "RetroLib/Concepts/Iterators.h"
#include "RetroLib/Concepts/Tuples.h"
#include "RetroLib/Functional/ExtensionMethods.h"
#include "RetroLib/Ranges/FeatureBridge.h"
#include "RetroLib/Ranges/RangeBasics.h"
#include "RetroLib/Utils/NonPropagatingCache.h"
#endif

#ifndef RETROLIB_EXPORT
#define RETROLIB_EXPORT
#endif

namespace retro::ranges {

    template <typename T, size_t N>
    concept ReturnableElement = std::is_reference_v<T> || std::move_constructible<std::tuple_element<N, T>>;

    /**
     * @class ElementsView
     * @brief A view over a range that extracts the Nth element from each tuple in the range.
     *
     * Inherits from `std::ranges::view_interface`, providing an interface for ranges.
     * The main responsibility of the `ElementsView` is to provide access to a sub-element
     * within tuples or tuple-like elements within a given ranged container.
     *
     * @tparam R The range type over which this view operates.
     * @tparam N The index of the tuple element to access.
     */
    RETROLIB_EXPORT template <std::ranges::input_range R, size_t N>
        requires std::ranges::view<R> && HasTupleElement<std::ranges::range_value_t<R>, N> &&
                 HasTupleElement<std::remove_reference_t<std::ranges::range_reference_t<R>>, N> &&
                 ReturnableElement<std::ranges::range_reference_t<R>, N>
    class ElementsView : public std::ranges::view_interface<ElementsView<R, N>> {

        template <bool IsConst>
        struct Sentinel;

        template <bool IsConst>
        struct Iterator {
            using value_type = std::decay_t<std::tuple_element_t<N, std::ranges::range_value_t<R>>>;
            using difference_type = std::ranges::range_difference_t<R>;
            using single_pass = std::bool_constant<SinglePassIterator<std::ranges::iterator_t<R>>>;

          private:
            friend struct Iterator<!IsConst>;
            friend struct Sentinel<!IsConst>;
            friend struct Sentinel<IsConst>;
            template <typename T>
            using ConstifyIf = std::conditional_t<IsConst, std::add_const_t<T>, T>;
            using ElementsViewType = ConstifyIf<ElementsView>;
            using IteratorType = std::ranges::iterator_t<ConstifyIf<R>>;

          public:
            constexpr Iterator() = default;

            constexpr Iterator(ElementsViewType &view, BeginTag) : view(&view), it(std::ranges::begin(view.range)) {
            }

            constexpr Iterator(ElementsViewType &view, EndTag) : view(&view), it(std::ranges::end(view.range)) {
            }

            template <bool Other>
                requires IsConst && (!Other)
            explicit constexpr Iterator(Iterator<Other> other) : view(other.view), it(std::move(other.it)) {
            }

            [[nodiscard]] constexpr decltype(auto) operator*() const noexcept(noexcept(get<N>(*it)))
                requires std::is_reference_v<std::ranges::range_reference_t<R>>
            {
                return get<N>(*it);
            }

            [[nodiscard]] constexpr decltype(auto) operator*() const noexcept(
                std::is_nothrow_move_constructible_v<std::tuple_element_t<N, std::ranges::range_reference_t<R>>> //
                && noexcept(get<N>(*it))) {
                using ElementType = std::remove_cv_t<std::tuple_element_t<N, std::ranges::range_reference_t<R>>>;
                return static_cast<ElementType>(get<N>(*it));
            }

            constexpr Iterator &operator++() noexcept(noexcept(++it)) {
                ++it;
                return *this;
            }

            constexpr void operator++(int) noexcept(noexcept(++it)) {
                ++it;
            }

            constexpr Iterator
            operator++(int) noexcept(noexcept(++it) && std::is_nothrow_copy_constructible_v<std::ranges::iterator_t<R>>)
                requires std::ranges::forward_range<R>
            {
                auto temp = *this;
                ++it;
                return temp;
            }

            constexpr Iterator &operator--() noexcept(noexcept(--it))
                requires std::ranges::bidirectional_range<R>
            {
                --it;
                return *this;
            }

            constexpr Iterator
            operator--(int) noexcept(noexcept(--it) && std::is_nothrow_copy_constructible_v<std::ranges::iterator_t<R>>)
                requires std::ranges::bidirectional_range<R>
            {
                auto temp = *this;
                --it;
                return temp;
            }

            constexpr Iterator &operator+=(const difference_type offset) noexcept(noexcept(it += offset))
                requires std::ranges::random_access_range<R>
            {
                it += offset;
                return *this;
            }

            constexpr Iterator &operator-=(const difference_type offset) noexcept(noexcept(it -= offset))
                requires std::ranges::random_access_range<R>
            {
                it -= offset;
                return *this;
            }

            [[nodiscard]] constexpr decltype(auto) operator[](const difference_type index) const
                noexcept(noexcept(get<N>(*(it + index))))
                requires std::ranges::random_access_range<R> && std::is_reference_v<std::ranges::range_reference_t<R>>
            {
                return get<N>(*(it + index));
            }

            [[nodiscard]] constexpr decltype(auto) operator[](const difference_type index) const noexcept(
                std::is_nothrow_move_constructible_v<std::tuple_element_t<N, std::ranges::range_reference_t<R>>> //
                && noexcept(get<N>(*(it + index))))
                requires std::ranges::random_access_range<R>
            {
                using ElementType = std::remove_cv_t<std::tuple_element_t<N, std::ranges::range_reference_t<R>>>;
                return static_cast<ElementType>(get<N>(*(it + index)));
            }

            [[nodiscard]] friend constexpr bool
            operator==(const Iterator &left, const Iterator &right) noexcept(noexcept(left.it == right.it))
                requires std::equality_comparable<std::ranges::iterator_t<R>>
            {
                return left.it == right.it;
            }

            [[nodiscard]] friend constexpr bool operator<(const Iterator &left,
                                                          const Iterator &right) noexcept(noexcept(left.it < right.it))
                requires std::ranges::random_access_range<R>
            {
                return left.it < right.it;
            }
            [[nodiscard]] friend constexpr bool operator>(const Iterator &left,
                                                          const Iterator &right) noexcept(noexcept(left.it < right.it))
                requires std::ranges::random_access_range<R>
            {
                return right < left;
            }
            [[nodiscard]] friend constexpr bool operator<=(const Iterator &left,
                                                           const Iterator &right) noexcept(noexcept(left.it < right.it))
                requires std::ranges::random_access_range<R>
            {
                return !(right < left);
            }
            [[nodiscard]] friend constexpr bool operator>=(const Iterator &left,
                                                           const Iterator &right) noexcept(noexcept(left.it < right.it))
                requires std::ranges::random_access_range<R>
            {
                return !(left < right);
            }

            [[nodiscard]] friend constexpr auto
            operator<=>(const Iterator &left, const Iterator &right) noexcept(noexcept(left.it <=> right.it))
                requires std::ranges::random_access_range<R> && std::three_way_comparable<std::ranges::iterator_t<R>>
            {
                return left.it <=> right.it;
            }

            [[nodiscard]] friend constexpr Iterator
            operator+(const Iterator &it, const difference_type offset) noexcept(
                noexcept(std::declval<std::ranges::iterator_t<R> &>() += offset) &&
                std::is_nothrow_copy_constructible_v<std::ranges::iterator_t<R>>)
                requires std::ranges::random_access_range<R>
            {
                auto copy = it;
                copy.it += offset;
                return copy;
            }

            [[nodiscard]] friend constexpr Iterator
            operator+(const difference_type offset,
                      const Iterator &it) noexcept(noexcept(std::declval<std::ranges::iterator_t<R> &>() += offset) &&
                                                   std::is_nothrow_copy_constructible_v<std::ranges::iterator_t<R>>)
                requires std::ranges::random_access_range<R>
            {
                auto copy = it;
                copy.it += offset;
                return copy;
            }

            [[nodiscard]] friend constexpr Iterator
            operator-(const Iterator &it, const difference_type offset) noexcept(
                noexcept(std::declval<std::ranges::iterator_t<R> &>() -= offset) &&
                std::is_nothrow_copy_constructible_v<std::ranges::iterator_t<R>>)
                requires std::ranges::random_access_range<R>
            {
                auto copy = it;
                copy.it -= offset;
                return copy;
            }

            [[nodiscard]] friend constexpr difference_type
            operator-(const Iterator &left, const Iterator &right) noexcept(noexcept(left.it - right.it))
                requires std::sized_sentinel_for<std::ranges::iterator_t<R>, std::ranges::iterator_t<R>>
            {
                return left.it - right.it;
            }

          private:
            ElementsViewType *view;
            std::ranges::iterator_t<ConstifyIf<R>> it;
        };

        template <bool IsConst>
        struct Sentinel {
          private:
            friend struct Sentinel<!IsConst>;
            friend struct Iterator<IsConst>;
            template <typename T>
            using ConstifyIf = std::conditional_t<IsConst, std::add_const_t<T>, T>;
            using ElementsViewType = ConstifyIf<ElementsView>;

          public:
            constexpr Sentinel() = default;
            explicit constexpr Sentinel(ElementsViewType &view) : end_element(std::ranges::end(view.range)) {
            }

            template <bool Other>
                requires IsConst && (!Other)
            explicit constexpr Sentinel(Sentinel<Other> other) : end_element(std::move(other.end_element)) {
            }

            template <bool Other>
                requires std::sentinel_for<std::ranges::sentinel_t<R>, typename Iterator<Other>::IteratorType>
            [[nodiscard]] friend constexpr bool
            operator==(const Iterator<Other> &left,
                       const Sentinel &right) noexcept(noexcept(get_iterator_value(left) == right.end_element)) {
                return get_iterator_value(left) == right.end_element;
            }

            template <bool Other>
                requires std::sized_sentinel_for<std::ranges::sentinel_t<R>, typename Iterator<Other>::IteratorType>
            [[nodiscard]] friend constexpr std::ranges::range_difference_t<R>
            operator-(const Iterator<Other> &left,
                      const Sentinel &right) noexcept(noexcept(get_iterator_value(left) - right.end_element)) {
                return get_iterator_value(left) - right.end_element;
            }

            template <bool Other>
                requires std::sized_sentinel_for<std::ranges::sentinel_t<R>, typename Iterator<Other>::IteratorType>
            [[nodiscard]] friend constexpr std::ranges::range_difference_t<R>
            operator-(const Sentinel &left,
                      const Iterator<Other> &right) noexcept(noexcept(left.end_element - get_iterator_value(right))) {
                return left.end_element - get_iterator_value(right);
            }

          private:
            template <bool Other>
            static constexpr decltype(auto) get_iterator_value(const Iterator<Other> &it) noexcept {
                return it.it;
            }

            std::ranges::sentinel_t<ConstifyIf<R>> end_element;
        };

      public:
        /**
         * @brief Default constructor for the `ElementsView` class.
         *
         * This is a constexpr constructor that initializes an instance of the
         * `ElementsView` class with its default state. As it is marked with `constexpr`,
         * it can be evaluated at compile-time.
         */
        constexpr ElementsView() = default;

        /**
         * @brief Constructs an `ElementsView` with the given range.
         *
         * This is a constexpr explicit constructor that initializes an instance of the
         * `ElementsView` class by taking ownership of the provided range.
         *
         * @param range The range to be used for initializing the `ElementsView` instance.
         */
        constexpr explicit ElementsView(R range) : range(std::move(range)) {
        }

        /**
         * @brief Retrieves the base range.
         *
         * This method returns the base range `range` when the type `R` satisfies the
         * `std::copy_constructible` concept. It is a `constexpr` function, meaning it
         * can be evaluated at compile-time if all used expressions are also constant
         * expressions.
         *
         * @return The base range of type `R`.
         */
        constexpr R base() const &
            requires std::copy_constructible<R>
        {
            return range;
        }

        /**
         * @brief Returns the underlying range.
         *
         * This method moves the stored range and returns it. As it is marked
         * with `constexpr`, this operation can be evaluated at compile-time.
         * The method applies only to rvalue instances of the class.
         *
         * @return `R` The moved underlying range.
         */
        constexpr R base() && {
            return std::move(range);
        }

        /**
         * @brief Returns an iterator to the beginning of the range.
         *
         * This function provides a `constexpr` iterator to the start of the range
         * when `SimpleView<R>` is not satisfied. The operation ensures exception safety
         * if the underlying range guarantees it. The returned iterator allows iteration
         * over the elements of the range it wraps.
         *
         * @return A constexpr object of type `Iterator<false>` pointing to the first element
         *         of the range.
         */
        [[nodiscard]] constexpr Iterator<false>
        begin() noexcept(noexcept(std::ranges::begin(range)) &&
                         std::is_nothrow_move_constructible_v<std::ranges::iterator_t<R>>)
            requires(!SimpleView<R>)
        {
            return Iterator<false>(*this, BeginTag());
        }

        /**
         * @brief Returns a `constexpr` iterator to the beginning of the range.
         *
         * This method provides a read-only iterator pointing to the first element of the range.
         * It is marked as `[[nodiscard]]` to encourage the caller to use the returned iterator.
         * The operation is `noexcept` if the underlying range operations are also `noexcept`,
         * and is constrained to work only if `const R` satisfies the `std::ranges::range` concept.
         *
         * @return A `constexpr` iterator positioned at the beginning of the range.
         */
        [[nodiscard]] constexpr Iterator<true> begin() const
            noexcept(noexcept(std::ranges::begin(range)) &&
                     std::is_nothrow_move_constructible_v<std::ranges::iterator_t<const R>>)
            requires std::ranges::range<const R>
        {
            return Iterator<true>(*this, BeginTag());
        }

        /**
         * @brief Retrieves the end iterator or sentinel for the range.
         *
         * This function computes the appropriate end representation for the range, either as an iterator
         * or a sentinel, depending on whether the range is a common range or not. It ensures noexcept
         * propagation and satisfies requirements for views that are not simple.
         *
         * @return The end iterator or sentinel for the range.
         */
        [[nodiscard]] constexpr auto end() noexcept(noexcept(std::ranges::end(range)) &&
                                                    std::is_nothrow_move_constructible_v<std::ranges::sentinel_t<R>>)
            requires(!SimpleView<R>)
        {
            if constexpr (std::ranges::common_range<R>) {
                return Iterator<false>(*this, EndTag());
            } else {
                return Sentinel<false>(*this);
            }
        }

        /**
         * @brief Returns an iterator or sentinel representing the end of the range.
         *
         * This function conditionally returns either an `Iterator` or `Sentinel` object
         * depending on whether the range is a common range or not. It ensures that
         * the returned object corresponds to the end of the range.
         *
         * @return An `Iterator` or `Sentinel` representing the end of the range.
         */
        [[nodiscard]] constexpr auto end() const
            noexcept(noexcept(std::ranges::end(range)) &&
                     std::is_nothrow_move_constructible_v<std::ranges::sentinel_t<const R>>)
            requires std::ranges::range<const R>
        {
            if constexpr (std::ranges::common_range<const R>) {
                return Iterator<true>(*this, EndTag());
            } else {
                return Sentinel<true>(*this);
            }
        }

        /**
         * @brief Retrieves the size of the range.
         *
         * This function returns the size of the range if the range satisfies the
         * `std::ranges::sized_range` constraint. It is marked as `constexpr` to allow
         * compile-time evaluation and `[[nodiscard]]` to encourage the caller to use
         * the returned value.
         *
         * @return The size of the range as determined by `std::ranges::size`.
         */
        [[nodiscard]] constexpr auto size() noexcept(noexcept(std::ranges::size(range)))
            requires std::ranges::sized_range<R>
        {
            return std::ranges::size(range);
        }

        /**
         * @brief Retrieves the size of the range.
         *
         * This method returns the number of elements in the range. It is marked as `constexpr`,
         * allowing it to be evaluated at compile-time if possible, and `[[nodiscard]]`, indicating
         * that the returned value should not be ignored. The method is only enabled if the range
         * satisfies the `std::ranges::sized_range` concept.
         *
         * @return The size of the range as determined by `std::ranges::size(range)`.
         */
        [[nodiscard]] constexpr auto size() const noexcept(noexcept(std::ranges::size(range)))
            requires std::ranges::sized_range<const R>
        {
            return std::ranges::size(range);
        }

      private:
        R range;
    };

    namespace views {
        template <size_t N>
        struct ElementsFunction {
            /**
             * @brief Invokes the function call operator to transform the given range into an `ElementsView`.
             *
             * This constexpr operator applies a transformation to the input range, forwarding it
             * and creating an instance of `ElementsView` with the specified range and element selection.
             *
             * @tparam R The type of the input range being processed.
             * @param range The input range to be transformed. The range is forwarded to preserve value category.
             * @return A new `ElementsView` instance that encapsulates the processed range.
             */
            template <std::ranges::input_range R>
                requires std::ranges::viewable_range<R> && HasTupleElement<std::ranges::range_value_t<R>, N> &&
                         HasTupleElement<std::remove_reference_t<std::ranges::range_reference_t<R>>, N> &&
                         ReturnableElement<std::ranges::range_reference_t<R>, N>
            constexpr auto operator()(R &&range) const {
                return ElementsView<std::ranges::views::all_t<R>, N>(std::ranges::views::all(std::forward<R>(range)));
            }
        };

        template <size_t N>
        constexpr ElementsFunction<N> elements_function;

        /**
         * @brief Creates an extension method for accessing specific tuple elements.
         *
         * This constexpr variable template utilizes the `extension_method` with the
         * `elements_function<N>` specialization to generate a view for accessing
         * specific elements of tuples (or similar range-based data structures).
         *
         * The resulting range adapter can be used to process or iterate through
         * a specific "field" or positional element across a collection of tuples,
         * maintaining compatibility with standard range concepts such as
         * `sized_range` and `random_access_range`.
         *
         * @tparam N Specifies the index of the tuple element to be accessed within
         *           the range-based data structure.
         */
        RETROLIB_EXPORT template <size_t N>
        constexpr auto elements = extension_method<elements_function<N>>();

        /**
         * @brief Creates a view selecting the elements at index 0 from the collection.
         *
         * This declares a compile-time constant view that, when applied to a suitable
         * collection, extracts only the first element (or key, in the case of maps)
         * from each entry. This is most commonly used in ranges to retrieve keys
         * or index-subset elements from collections such as maps or tuples.
         */
        RETROLIB_EXPORT constexpr auto keys = elements<0>;

        /**
         * @brief A `constexpr` initialization of the `values` variable using the `elements` utility.
         *
         * This variable applies the `elements<1>` function, presumably to extract or represent
         * a specific aspect of a data structure or range in a compile-time evaluable manner.
         */
        RETROLIB_EXPORT constexpr auto values = elements<1>;
    } // namespace views
} // namespace retro::ranges