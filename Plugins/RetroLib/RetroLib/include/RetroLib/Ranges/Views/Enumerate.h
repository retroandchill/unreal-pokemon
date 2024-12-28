/**
 * @file Enumerate.h
 * @brief Handles enumerating through a collection
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

    template <typename R>
    concept RangeWithMovableReference =
        std::ranges::input_range<R> && std::move_constructible<std::ranges::range_reference_t<R>> &&
        std::move_constructible<std::ranges::range_reference_t<R>>;

    /**
     * @class EnumerateView
     * A view adaptor that enumerates the elements of a given range, pairing each element with its zero-based index.
     * This view allows iteration over a range with elements wrapped along with their indices in a tuple.
     *
     * @tparam V The type of the underlying range to be enumerated.
     * It can be any range satisfying the constraints for views as defined by the Standard Library.
     */
    RETROLIB_EXPORT template <std::ranges::view V>
        requires RangeWithMovableReference<V>
    class EnumerateView : public std::ranges::view_interface<EnumerateView<V>> {

        template <bool Const>
        class Iterator {
            using Base = std::conditional_t<Const, const V, V>;

          public:
            using iterator_category = std::input_iterator_tag;
            using iterator_concept = std::conditional_t<
                std::ranges::random_access_range<Base>, std::random_access_iterator_tag,
                std::conditional_t<std::ranges::bidirectional_range<Base>, std::bidirectional_iterator_tag,
                                   std::conditional_t<std::ranges::forward_range<Base>, std::forward_iterator_tag,
                                                      std::input_iterator_tag>>>;
            using difference_type = std::ranges::range_difference_t<Base>;
            using value_type = std::tuple<difference_type, std::ranges::range_value_t<Base>>;

          private:
            using ReferenceType = std::tuple<difference_type, std::ranges::range_reference_t<Base>>;

          public:
            constexpr Iterator()
                requires std::default_initializable<std::ranges::iterator_t<Base>>
            = default;

            constexpr explicit(false) Iterator(Iterator<!Const> other)
                requires Const && std::convertible_to<std::ranges::iterator_t<V>, std::ranges::iterator_t<Base>>
                : current(std::move(other.current)), pos(other.pos) {
            }

          private:
            constexpr explicit Iterator(std::ranges::iterator_t<Base> current, difference_type pos)
                : current(std::move(current)), pos(pos) {
            }

          public:
            constexpr const std::ranges::iterator_t<Base> &base() const & noexcept {
                return current;
            }

            constexpr std::ranges::iterator_t<Base> base() && {
                return std::move(current);
            }

            constexpr difference_type index() const noexcept {
                return pos;
            }

            constexpr auto operator*() const
                noexcept(noexcept(*current) &&
                         std::is_nothrow_copy_constructible_v<std::ranges::range_reference_t<Base>>) {
                return ReferenceType(pos, *current);
            }

            constexpr auto operator[](difference_type n) const
                requires std::ranges::random_access_range<Base>
            {
                return ReferenceType(pos + n, current[n]);
            }

            constexpr Iterator &operator++() {
                ++current;
                ++pos;
                return *this;
            }

            constexpr void operator++(int) {
                ++current;
                ++pos;
            }

            constexpr Iterator operator++(int)
                requires std::ranges::forward_range<Base>
            {
                Iterator tmp = *this;
                ++*this;
                return tmp;
            }

            constexpr Iterator &operator--()
                requires std::ranges::bidirectional_range<Base>
            {
                --current;
                --pos;
                return *this;
            }

            constexpr Iterator operator--(int)
                requires std::ranges::bidirectional_range<Base>
            {
                Iterator tmp = *this;
                --*this;
                return tmp;
            }

            constexpr Iterator operator+=(difference_type n)
                requires std::ranges::random_access_range<Base>
            {
                current += n;
                pos += n;
                return *this;
            }

            constexpr Iterator operator-=(difference_type n)
                requires std::ranges::random_access_range<Base>
            {
                current -= n;
                pos -= n;
                return *this;
            }

            friend constexpr bool operator==(const Iterator &lhs, const Iterator &rhs) noexcept {
                return lhs.pos == rhs.pos;
            }

            friend constexpr std::strong_ordering operator<=>(const Iterator &lhs, const Iterator &rhs) noexcept {
                return lhs.pos <=> rhs.pos;
            }

            friend constexpr Iterator operator+(const Iterator &self, difference_type n)
                requires std::ranges::random_access_range<Base>
            {
                auto temp = self;
                temp += n;
                return temp;
            }

            friend constexpr Iterator operator+(difference_type n, const Iterator &self)
                requires std::ranges::random_access_range<Base>
            {
                return self + n;
            }

            friend constexpr Iterator operator-(const Iterator &self, difference_type n)
                requires std::ranges::random_access_range<Base>
            {
                auto temp = self;
                temp -= n;
                return temp;
            }

            friend constexpr difference_type operator-(const Iterator &lhs, const Iterator &rhs)
                requires std::ranges::random_access_range<Base>
            {
                return lhs.pos - rhs.pos;
            }

            friend constexpr auto iter_move(const Iterator &self) noexcept(
                noexcept(std::ranges::iter_move(self.current)) &&
                std::is_nothrow_move_constructible_v<std::ranges::range_rvalue_reference_t<Base>>) {
                using Tuple = std::tuple<difference_type, std::ranges::range_rvalue_reference_t<Base>>;
                return Tuple(self.pos, std::ranges::iter_move(self.current));
            }

          private:
            friend class Iterator<!Const>;
            friend class EnumerateView;

            std::ranges::iterator_t<Base> current;
            difference_type pos;
        };

        template <bool Const>
        class Sentinel {
            using Base = std::conditional_t<Const, const V, V>;

          public:
            constexpr Sentinel() = default;

            constexpr explicit(false) Sentinel(Iterator<!Const> other)
                requires Const && std::convertible_to<std::ranges::sentinel_t<V>, std::ranges::sentinel_t<Base>>
                : end(std::move(other.end)) {
            }

          private:
            constexpr explicit Sentinel(std::ranges::sentinel_t<Base> end) : end(std::move(end)) {
            }

          public:
            constexpr std::ranges::sentinel_t<Base> base() const noexcept {
                return end;
            }

            friend constexpr bool operator==(const Iterator<Const> &lhs, const Sentinel &rhs) noexcept {
                return lhs.base() == rhs.end;
            }

            template <bool OtherConst>
                requires std::sized_sentinel_for<std::ranges::sentinel_t<Base>,
                                                 std::ranges::iterator_t<MaybeConst<OtherConst, V>>>
            friend constexpr auto operator-(const Iterator<OtherConst> &lhs, const Sentinel &rhs) {
                return lhs.base() - rhs.end;
            }

            template <bool OtherConst>
                requires std::sized_sentinel_for<std::ranges::sentinel_t<Base>,
                                                 std::ranges::iterator_t<MaybeConst<OtherConst, V>>>
            friend constexpr auto operator-(const Sentinel &lhs, const Iterator<OtherConst> &rhs) {
                return lhs.end - rhs.base();
            }

          private:
            std::ranges::sentinel_t<Base> end;
        };

      public:
        /**
         * @brief Default constructor for the EnumerateView class.
         *
         * This constructor creates an EnumerateView object with default initialization.
         *
         * @return A default-initialized EnumerateView object.
         */
        constexpr EnumerateView() = default;

        /**
         * @brief Constructs an EnumerateView object with an explicitly provided view.
         *
         * This constructor initializes an EnumerateView object with the given view parameter.
         * It is marked as constexpr for potential compile-time evaluation and explicit to
         * prevent unintended implicit conversions.
         *
         * @param view The view object used to initialize the EnumerateView.
         */
        constexpr explicit EnumerateView(V view) : view(view) {
        }

        /**
         * @brief Returns the base view of the current object.
         *
         * This function retrieves the base view as a constant reference, requiring the type `V` to be copy
         * constructible.
         *
         * @return The base view as a constant reference.
         */
        constexpr V base() const &
            requires std::copy_constructible<V>
        {
            return view;
        }

        /**
         * @brief Retrieves the base view in a rvalue context.
         *
         * This function returns the base view `V` when the current object is an rvalue.
         *
         * @return The base view `V`.
         */
        constexpr V base() && {
            return view;
        }

        /**
         * @brief Returns an iterator to the beginning of the view.
         *
         * This method provides an iterator to the start of the view when the
         * SimpleView concept is not satisfied.
         *
         * @return An iterator pointing to the beginning of the view.
         */
        constexpr auto begin()
            requires(!SimpleView<V>)
        {
            return Iterator<false>(std::ranges::begin(view), 0);
        }

        /**
         * @brief Returns an iterator to the beginning of the range.
         *
         * This method provides a constant iterator to the first element of the range,
         * ensuring compatibility with ranges that have movable references and meet the
         * required constraints.
         *
         * @return An iterator to the beginning of the range.
         */
        constexpr auto begin() const
            requires RangeWithMovableReference<const V>
        {
            return Iterator<true>(std::ranges::begin(view), 0);
        }

        /**
         * @brief Determines the end marker of the range for the view.
         *
         * This function provides the appropriate end representation for the view,
         * creating either an Iterator or Sentinel type depending on the properties
         * of the underlying range. The choice of representation depends on whether
         * the range satisfies forward_range, common_range, and sized_range concepts.
         *
         * @return The appropriate representation of the end marker for the view,
         *         either as an Iterator<false> or Sentinel<false>.
         */
        constexpr auto end()
            requires(!SimpleView<V>)
        {
            if constexpr (std::ranges::forward_range<V> && std::ranges::common_range<V> &&
                          std::ranges::sized_range<V>) {
                return Iterator<false>(std::ranges::end(view), std::ranges::distance(view));
            } else {
                return Sentinel<false>(std::ranges::end(view));
            }
        }

        /**
         * @brief Obtains the end iterator or sentinel for the range.
         *
         * This constexpr function computes the appropriate end representation for the range,
         * depending on whether the range satisfies specific constraints such as being a
         * forward range, common range, and sized range.
         *
         * @return An iterator or sentinel representing the end of the range.
         *         - If the range is a forward range, common range, and sized range,
         *           it returns an Iterator object initialized with the end of the range and its distance.
         *         - Otherwise, it returns a Sentinel object initialized with the end of the range.
         *
         * @note This function requires the range to meet the criteria specified by
         *       `RangeWithMovableReference<const V>`.
         */
        constexpr auto end() const
            requires RangeWithMovableReference<const V>
        {
            if constexpr (std::ranges::forward_range<V> && std::ranges::common_range<V> &&
                          std::ranges::sized_range<V>) {
                return Iterator<true>(std::ranges::end(view), std::ranges::distance(view));
            } else {
                return Sentinel<true>(std::ranges::end(view));
            }
        }

        /**
         * @brief Returns the size of the view if the underlying range satisfies the SizedRange concept.
         *
         * This function calculates and returns the size of the view using the std::ranges::size
         * function, provided that the underlying range meets the requirements of a sized range.
         *
         * @return The size of the view as determined by std::ranges::size.
         * @throws No explicit exception is thrown, but usage requires the underlying range to satisfy
         *         the std::ranges::sized_range concept.
         */
        constexpr auto size()
            requires std::ranges::sized_range<V>
        {
            return std::ranges::size(view);
        }

        /**
         * @brief Retrieves the size of the range if the view satisfies the sized_range constraint.
         *
         * This method returns the size of the range for the given view, provided the view
         * meets the requirements of a sized_range.
         *
         * @return The size of the range as determined by std::ranges::size.
         */
        constexpr auto size() const
            requires std::ranges::sized_range<const V>
        {
            return std::ranges::size(view);
        }

      private:
        friend class EnumerateView;

        V view;
    };

    namespace views {
        struct EnumerateInvoker {
            /**
             * @brief Function call operator to create an EnumerateView from a given range.
             *
             * This operator enables the creation of an EnumerateView object by forwarding the
             * provided range to its constructor.
             *
             * @param range The input range to be used for creating an EnumerateView.
             *              The parameter is forwarded to the EnumerateView constructor.
             * @return An EnumerateView object constructed from the provided range.
             */
            template <std::ranges::viewable_range R>
                requires RangeWithMovableReference<std::ranges::views::all_t<R>>
            constexpr auto operator()(R &&range) const {
                return EnumerateView<std::ranges::views::all_t<R>>(std::ranges::views::all(std::forward<R>(range)));
            }
        };

        /**
         * @brief A helper constant for enabling enumeration over a given sequence.
         *
         * The `enumerate` constant is used to apply the `EnumerateInvoker` behavior
         * through `extension_method`. It provides a mechanism to iterate over
         * elements in a sequence alongside their respective indices.
         *
         * @note This is a `constexpr` value, ensuring compile-time initialization
         * and usage for eligible contexts.
         */
        RETROLIB_EXPORT constexpr auto enumerate = extension_method<EnumerateInvoker{}>();
    } // namespace views

    RETROLIB_EXPORT template <std::ranges::view V, std::ranges::random_access_range R>
        requires RangeWithMovableReference<V> && RangeWithMovableReference<R> && std::ranges::view<R> &&
                 std::convertible_to<std::ranges::range_reference_t<V>, std::ranges::range_difference_t<R>>
    /**
     * @brief A view that provides reverse enumeration over a range.
     *
     * The ReverseEnumerateView class allows enumeration of a range of indices creating a tuple of those indices with
     * the element obtained from an input range.
     *
     * @tparam V The type of the index view.
     * @tparam R The type of the data range to be enumerated.
     */
    class ReverseEnumerateView : public std::ranges::view_interface<ReverseEnumerateView<V, R>> {

        template <bool Const>
        class Iterator {
            using Base = std::conditional_t<Const, const V, V>;
            using Viewed = std::conditional_t<Const, const R, R>;

          public:
            using iterator_category = std::input_iterator_tag;
            using iterator_concept = std::conditional_t<
                std::ranges::random_access_range<Base>, std::random_access_iterator_tag,
                std::conditional_t<std::ranges::bidirectional_range<Base>, std::bidirectional_iterator_tag,
                                   std::conditional_t<std::ranges::forward_range<Base>, std::forward_iterator_tag,
                                                      std::input_iterator_tag>>>;
            using difference_type = std::ranges::range_difference_t<Base>;
            using value_type = std::tuple<std::ranges::range_value_t<Base>, std::ranges::range_value_t<Viewed>>;

          private:
            using ReferenceType = std::tuple<std::ranges::range_value_t<Base>, std::ranges::range_reference_t<Viewed>>;

          public:
            constexpr Iterator()
                requires std::default_initializable<std::ranges::iterator_t<Base>> &&
                             std::default_initializable<std::ranges::iterator_t<Viewed>>
            = default;

            constexpr explicit(false) Iterator(Iterator<!Const> other)
                requires Const && std::convertible_to<std::ranges::iterator_t<V>, std::ranges::iterator_t<Base>>
                : current(std::move(other.current)), viewed(std::move(other.viewed)) {
            }

          private:
            constexpr explicit Iterator(std::ranges::iterator_t<Base> current, Viewed &viewed)
                : current(std::move(current)), viewed(std::ranges::begin(viewed)) {
            }

          public:
            constexpr const std::ranges::iterator_t<Base> &base() const & noexcept {
                return current;
            }

            constexpr std::ranges::iterator_t<Base> base() && {
                return std::move(current);
            }

            constexpr auto operator*() const
                noexcept(noexcept(*current) && noexcept(viewed[*current]) &&
                         std::is_nothrow_copy_constructible_v<std::ranges::range_reference_t<Base>>) {
                return ReferenceType(*current, viewed[*current]);
            }

            constexpr auto operator[](difference_type n) const
                requires std::ranges::random_access_range<Base>
            {
                return ReferenceType(*current + n, viewed[*current + n]);
            }

            constexpr Iterator &operator++() {
                ++current;
                return *this;
            }

            constexpr void operator++(int) {
                ++current;
            }

            constexpr Iterator operator++(int)
                requires std::ranges::forward_range<Base>
            {
                Iterator tmp = *this;
                ++*this;
                return tmp;
            }

            constexpr Iterator &operator--()
                requires std::ranges::bidirectional_range<Base>
            {
                --current;
                return *this;
            }

            constexpr Iterator operator--(int)
                requires std::ranges::bidirectional_range<Base>
            {
                Iterator tmp = *this;
                --*this;
                return tmp;
            }

            constexpr Iterator operator+=(difference_type n)
                requires std::ranges::random_access_range<Base>
            {
                current += n;
                return *this;
            }

            constexpr Iterator operator-=(difference_type n)
                requires std::ranges::random_access_range<Base>
            {
                current -= n;
                return *this;
            }

            friend constexpr bool operator==(const Iterator &lhs, const Iterator &rhs) noexcept {
                return lhs.current == rhs.current;
            }

            friend constexpr std::strong_ordering operator<=>(const Iterator &lhs, const Iterator &rhs) noexcept {
                return lhs.current <=> rhs.current;
            }

            friend constexpr Iterator operator+(const Iterator &self, difference_type n)
                requires std::ranges::random_access_range<Base>
            {
                auto temp = self;
                temp += n;
                return temp;
            }

            friend constexpr Iterator operator+(difference_type n, const Iterator &self)
                requires std::ranges::random_access_range<Base>
            {
                return self + n;
            }

            friend constexpr Iterator operator-(const Iterator &self, difference_type n)
                requires std::ranges::random_access_range<Base>
            {
                auto temp = self;
                temp -= n;
                return temp;
            }

            friend constexpr difference_type operator-(const Iterator &lhs, const Iterator &rhs)
                requires std::ranges::random_access_range<Base>
            {
                return lhs.current - rhs.current;
            }

          private:
            friend class Iterator<!Const>;
            friend class ReverseEnumerateView;

            std::ranges::iterator_t<Base> current;
            std::ranges::iterator_t<Viewed> viewed;
        };

        template <bool Const>
        class Sentinel {
            using Base = std::conditional_t<Const, const V, V>;

          public:
            constexpr Sentinel() = default;

            constexpr explicit(false) Sentinel(Iterator<!Const> other)
                requires Const && std::convertible_to<std::ranges::sentinel_t<V>, std::ranges::sentinel_t<Base>>
                : end(std::move(other.end)) {
            }

          private:
            constexpr explicit Sentinel(std::ranges::sentinel_t<Base> end) : end(std::move(end)) {
            }

          public:
            constexpr std::ranges::sentinel_t<Base> base() const noexcept {
                return end;
            }

            friend constexpr bool operator==(const Iterator<Const> &lhs, const Sentinel &rhs) noexcept {
                return lhs.base() == rhs.end;
            }

            template <bool OtherConst>
                requires std::sized_sentinel_for<std::ranges::sentinel_t<Base>,
                                                 std::ranges::iterator_t<MaybeConst<OtherConst, V>>>
            friend constexpr auto operator-(const Iterator<OtherConst> &lhs, const Sentinel &rhs) {
                return lhs.base() - rhs.end;
            }

            template <bool OtherConst>
                requires std::sized_sentinel_for<std::ranges::sentinel_t<Base>,
                                                 std::ranges::iterator_t<MaybeConst<OtherConst, V>>>
            friend constexpr auto operator-(const Sentinel &lhs, const Iterator<OtherConst> &rhs) {
                return lhs.end - rhs.base();
            }

          private:
            std::ranges::sentinel_t<Base> end;
        };

    public:
        /**
         * @brief Default constructor for the ReverseEnumerateView class.
         *
         * This constructor enables the creation of a default-initialized ReverseEnumerateView object,
         * provided that the underlying type V satisfies the default_initializable requirement.
         *
         * @return A default-initialized ReverseEnumerateView object.
         */
        constexpr ReverseEnumerateView()
            requires std::default_initializable<V>
        = default;

        /**
         * @brief Constructs a ReverseEnumerateView object with specified view and range.
         *
         * This constructor initializes a ReverseEnumerateView object with the given view and range,
         * moving the provided values into the corresponding member variables.
         *
         * @param view The view to be used for enumeration.
         * @param range The range associated with the enumeration.
         */
        constexpr explicit ReverseEnumerateView(V view, R range) : indices(std::move(view)), range(std::move(range)) {
        }

        /**
         * @brief Retrieves the base value of the object.
         *
         * This method returns the base value, `indices`, while ensuring that the
         * `V` type satisfies the `std::copy_constructible` requirement.
         *
         * @return The base value of type `V`.
         */
        constexpr V base() const &
            requires std::copy_constructible<V>
        {
            return indices;
        }

        /**
         * @brief Returns the base object by moving the indices.
         *
         * This function provides access to the base object, transferring ownership
         * of the underlying indices through a move operation.
         *
         * @return The moved base object of type V.
         */
        constexpr V base() && {
            return std::move(indices);
        }

        /**
         * @brief Begins iteration over the range with a custom iterator.
         *
         * This method initializes and returns a custom iterator for the
         * range, starting from the beginning of the indices and the range.
         * The method is only enabled when the type `V` does not satisfy
         * the `SimpleView` concept.
         *
         * @return An initialized custom iterator for iterating over the range.
         */
        constexpr auto begin()
            requires(!SimpleView<V>)
        {
            return Iterator<false>(std::ranges::begin(indices), range);
        }

        /**
         * @brief Retrieves an iterator to the beginning of the range.
         *
         * This method returns an iterator to the first element of the range, assuming the conditions
         * for the range and iterator requirements are met.
         *
         * @return An iterator to the beginning of the range.
         */
        constexpr auto begin() const
            requires RangeWithMovableReference<const V> && std::ranges::random_access_range<const V>
        {
            return Iterator<true>(std::ranges::begin(indices), range);
        }

        /**
         * @brief Retrieves the end iterator or sentinel for the view, conditional on the view's properties.
         *
         * This function returns either an iterator or a sentinel for the view, depending on whether
         * the underlying view satisfies the forward range concept. It uses the conditional behavior
         * of ranges to determine the appropriate return type.
         *
         * @return The end iterator or sentinel for the view.
         */
        constexpr auto end()
            requires(!SimpleView<V>)
        {
            if constexpr (std::ranges::forward_range<V>) {
                return Iterator<false>(std::ranges::end(indices), range);
            } else {
                return Sentinel<false>(std::ranges::end(indices));
            }
        }

        /**
         * @brief Provides access to the end of the range in a constant context.
         *
         * This function returns an iterator or sentinel representing the end of the range,
         * depending on the underlying range type and its properties. It utilizes constraints
         * to ensure compatibility with ranges that support movable references and are random-access ranges.
         *
         * @return An iterator or sentinel representing the end of the range.
         */
        constexpr auto end() const
            requires RangeWithMovableReference<const V> && std::ranges::random_access_range<const V>
        {
            if constexpr (std::ranges::forward_range<V>) {
                return Iterator<true>(std::ranges::end(indices), range);
            } else {
                return Sentinel<true>(std::ranges::end(indices));
            }
        }

        /**
         * @brief Retrieves the size of the range if the range satisfies the sized_range concept.
         *
         * This function computes and returns the size of the range
         * represented by the indices member.
         *
         * @return The size of the range as determined by std::ranges::size.
         *         The return type is dependent on the range's size.
         */
        constexpr auto size()
            requires std::ranges::sized_range<V>
        {
            return std::ranges::size(indices);
        }

        /**
         * @brief Computes the size of the range.
         *
         * This method computes and returns the size of the range if the underlying
         * range satisfies the `std::ranges::sized_range` constraint.
         *
         * @return The size of the range as computed by `std::ranges::size`.
         */
        constexpr auto size() const
            requires std::ranges::sized_range<const V>
        {
            return std::ranges::size(indices);
        }

      private:
        V indices;
        R range;
    };

    namespace views {
        struct ReverseEnumerateInvoker {
            template <std::ranges::viewable_range V, std::ranges::random_access_range R>
                requires std::ranges::view<std::ranges::views::all_t<V>> && RangeWithMovableReference<V> &&
                         RangeWithMovableReference<R> && std::ranges::view<std::ranges::views::all_t<R>> &&
                         std::convertible_to<std::ranges::range_reference_t<V>, std::ranges::range_difference_t<R>>
            /**
             * @brief Constructs a ReverseEnumerateView from the given indices and range.
             *
             * This function operator creates a ReverseEnumerateView by applying `std::ranges::views::all`
             * to the provided indices and range parameters. The indices and range are forwarded to ensure
             * proper handling of value categories.
             *
             * @tparam V The type of the indices container.
             * @tparam R The type of the range container.
             * @param indices The container representing the indices to be associated with the range.
             * @param range The container representing the range of elements to be enumerated.
             * @return A ReverseEnumerateView constructed from the given indices and range.
             */
            constexpr auto operator()(V &&indices, R &&range) const {
                return ReverseEnumerateView(std::ranges::views::all(std::forward<V>(indices)),
                                            std::ranges::views::all(std::forward<R>(range)));
            }
        };

        /**
         * @brief Provides a reverse enumerate view as an extension method.
         *
         * This constant enables the creation of a range view that pairs elements
         * of a given range with their indices in reverse order. It is typically
         * used with ranges algorithms to enumerate elements and their indices
         * in reverse while processing a container or a range.
         *
         * @return A reverse enumerate view invokable for use in range operations.
         */
        RETROLIB_EXPORT constexpr auto reverse_enumerate = extension_method<ReverseEnumerateInvoker{}>;
    } // namespace views
} // namespace retro::ranges