/**
 * @file Concat.h
 * @brief View for concatenating two or more dissimilar ranges.
 *
 * @author Retro & Chill
 * https://github.com/retroandchill
 */
#pragma once

#if !RETROLIB_WITH_MODULES
#include "RetroLib/Concepts/Iterators.h"
#include "RetroLib/Ranges/Concepts/Concatable.h"
#include "RetroLib/Ranges/RangeBasics.h"
#include "RetroLib/Utils/Unreachable.h"
#include "RetroLib/Utils/Variant.h"

#include <tuple>
#include <variant>
#endif

#ifndef RETROLIB_EXPORT
#define RETROLIB_EXPORT
#endif

namespace retro::ranges {

    /**
     * @class ConcatView
     * @brief A custom view that concatenates multiple ranges into a single, continuous range.
     *
     * This class models a concatenation view over the provided ranges `R...`, allowing iteration
     * over their elements as if they formed a single range. It provides seamless iteration and
     * supports the standard concepts of ranges and iterators such as bidirectional iteration
     * and random access where applicable.
     *
     * @tparam R... The parameter pack of range types used in the concatenation view.
     *
     * The `ConcatView` class is implemented as a derived class of `std::ranges::view_interface`
     * to provide range-like behavior. It supports custom iterators and sentinels for iteration
     * across concatenated ranges.
     *
     * Key features and implementation details:
     * - Supports various iterator operations, including forward, bidirectional, and random-access iterators.
     * - Allows seamless transition between different ranges during iteration.
     * - Provides a unified view interface over non-contiguous or individually defined ranges.
     * - Memory-efficient, as it operates directly over the given ranges without copying their contents.
     *
     */
    RETROLIB_EXPORT template <std::ranges::input_range... R>
        requires(std::ranges::view<R> && ...) && (sizeof...(R) > 0) && Concatable<R...>
    class ConcatView : public std::ranges::view_interface<ConcatView<R...>> {
        using DifferenceType = std::common_type_t<std::ranges::range_difference_t<R>...>;
        static constexpr size_t ranges_size = sizeof...(R);
        std::tuple<R...> ranges;

        template <bool IsConst>
        struct Iterator;

        template <bool IsConst>
        struct Sentinel {
          private:
            friend struct Sentinel<!IsConst>;
            friend struct Iterator<IsConst>;
            template <typename T>
            using ConstifyIf = std::conditional_t<IsConst, std::add_const_t<T>, T>;
            using ConcatViewType = ConstifyIf<ConcatView>;
            std::ranges::sentinel_t<ConstifyIf<LastInPack<R...>>> end_element;

          public:
            constexpr Sentinel() = default;
            explicit constexpr Sentinel(ConcatViewType &view, EndTag)
                : end_element(std::ranges::end(std::get<ranges_size - 1>(view.ranges))) {
            }

            template <bool Other>
                requires IsConst && (!Other)
            explicit constexpr Sentinel(Sentinel<Other> other) : end_element(std::move(other.end_element)) {
            }
        };

        template <bool IsConst>
        struct Iterator {
            using difference_type = std::common_type_t<std::ranges::range_difference_t<R>...>;
            using value_type = ConcatValue<R...>;

          private:
            friend struct Iterator<!IsConst>;
            template <typename T>
            using ConstifyIf = std::conditional_t<IsConst, std::add_const_t<T>, T>;
            using ConcatViewType = ConstifyIf<ConcatView>;
            ConcatViewType *view;
            std::variant<std::ranges::iterator_t<ConstifyIf<R>>...> it;

            template <size_t N>
            constexpr void satisfy() {
                RETROLIB_ASSERT(it.index() == N);
                if constexpr (N < ranges_size - 1) {
                    if (std::get<N>(it) == std::ranges::end(std::get<N>(view->ranges))) {
                        it.template emplace<N + 1>(std::ranges::begin(std::get<N + 1>(view->ranges)));
                        satisfy<N + 1>();
                    }
                }
            }

            struct Next {
                Iterator *pos;
                template <std::input_iterator I, size_t N>
                constexpr void operator()(IndexedElement<I, N> it) const {
                    RETROLIB_ASSERT(it.get() != std::ranges::end(std::get<N>(pos->view->ranges)));
                    ++it.get();
                    pos->satisfy<N>();
                }
            };

            struct Prev {
                Iterator *pos;

                template <std::bidirectional_iterator I>
                constexpr void operator()(IndexedElement<I, 0> it) const {
                    RETROLIB_ASSERT(it.get() != std::ranges::begin(std::get<0>(pos->view->ranges)));
                    --it.get();
                }

                template <std::bidirectional_iterator I, size_t N>
                    requires(N != 0)
                constexpr void operator()(IndexedElement<I, N> it) const {
                    if (it.get() == std::ranges::begin(std::get<N>(pos->view->ranges))) {
                        auto &&rng = std::get<N - 1>(pos->view->ranges);
                        pos->it.template emplace<N - 1>(
                            std::ranges::next(std::ranges::begin(rng), std::ranges::end(rng)));
                        visit_index(*this, pos->it);
                    } else {
                        --it.get();
                    }
                }
            };

            struct AdvanceForward {
                Iterator *pos;
                difference_type n;

                template <std::random_access_iterator I>
                constexpr void operator()(IndexedElement<I, ranges_size - 1> it) const {
                    std::ranges::advance(it.get(), n);
                }

                template <std::random_access_iterator I, size_t N>
                constexpr void operator()(IndexedElement<I, N> it) const {
                    auto last = std::ranges::end(std::get<N>(pos->view->ranges));
                    auto rest = std::ranges::advance(it.get(), n, std::move(last));
                    pos->satisfy<N>();

                    if (rest != 0) {
                        visit_index<void>(AdvanceForward{pos, rest}, pos->it);
                    }
                }
            };

            struct AdvanceReverse {
                Iterator *pos;
                difference_type n;

                template <std::random_access_iterator I>
                constexpr void operator()(IndexedElement<I, 0> it) const {
                    std::ranges::advance(it.get(), n);
                }

                template <std::random_access_iterator I, std::size_t N>
                constexpr void operator()(IndexedElement<I, N> it) const {
                    auto first = std::ranges::begin(std::get<N>(pos->view->ranges));
                    if (it.get() == first) {
                        auto &&rng = std::get<N - 1>(pos->view->ranges);
                        pos->it.template emplace<N - 1>(
                            std::ranges::next(std::ranges::begin(rng), std::ranges::end(rng)));
                        visit_index(*this, pos->it);
                    } else {
                        auto rest = std::ranges::advance(it.get(), n, std::move(first));
                        if (rest != 0) {
                            visit_index<void>(AdvanceReverse{pos, rest}, pos->it);
                        }
                    }
                }
            };

            [[noreturn]] static difference_type distance_to(std::integral_constant<size_t, ranges_size>,
                                                            const Iterator &, const Iterator &) {
                RETROLIB_ASSERT(false);
                unreachable();
            }

            template <size_t N>
                requires(N < ranges_size)
            static difference_type distance_to(std::integral_constant<size_t, N>, const Iterator &from,
                                               const Iterator &to) {
                if (from.it.index() > N) {
                    return Iterator::distance_to(std::integral_constant<size_t, N + 1>{}, from, to);
                }

                if (from.it.index() == N) {
                    if (to.it.index() == N) {
                        return std::ranges::distance(std::get<N>(from.it), std::get<N>(to.it));
                    }

                    return std::ranges::distance(std::get<N>(from.it),
                                                 std::ranges::end(std::get<N>(from.view->ranges))) +
                           Iterator::distance_to(std::integral_constant<size_t, N + 1>{}, from, to);
                }
                if (from.it.index() < N && to.it.index() > N)
                    return std::ranges::distance(std::get<N>(from.view->ranges)) +
                           Iterator::distance_to(std::integral_constant<size_t, N + 1>{}, from, to);

                RETROLIB_ASSERT(to.it.index() == N);
                return std::ranges::distance(std::ranges::begin(std::get<N>(from.view->ranges)), std::get<N>(to.it));
            }

          public:
            using Reference = std::common_reference_t<std::ranges::range_reference_t<ConstifyIf<R>>...>;
            using single_pass = std::bool_constant<(SinglePassIterator<std::ranges::iterator_t<R>> || ...)>;

            constexpr Iterator() = default;

            constexpr Iterator(ConcatViewType &view, BeginTag)
                : view(&view), it(std::in_place_index<0>, std::ranges::begin(std::get<0>(view.ranges))) {
                satisfy<0>();
            }

            constexpr Iterator(ConcatViewType &view, EndTag)
                : view(&view),
                  it(std::in_place_index<ranges_size - 1>, std::ranges::end(std::get<ranges_size - 1>(view.ranges))) {
            }

            template <bool Other>
                requires IsConst && (!Other)
            explicit constexpr Iterator(Iterator<Other> other) : view(other.view), it(std::move(other.it)) {
            }

            constexpr Reference operator*() const {
                return std::visit<Reference>([]<typename T>(T &&it) -> decltype(auto) { return *std::forward<T>(it); },
                                             it);
            }

            constexpr Iterator &operator++() {
                visit_index(Next{this}, it);
                return *this;
            }

            constexpr auto operator++(int) {
                if constexpr ((std::ranges::forward_range<R> && ...)) {
                    auto tmp = *this;
                    ++*this;
                    return tmp;
                } else {
                    ++*this;
                }
            }

            constexpr bool operator==(const Iterator &pos) const
                requires std::equality_comparable<std::variant<std::ranges::iterator_t<ConstifyIf<R>>...>>
            {
                return it == pos.it;
            }

            constexpr bool operator==(const Sentinel<IsConst> &post) const {
                return it.index() == ranges_size - 1 && std::get<ranges_size - 1>(view->it) == post.end_element;
            }

            constexpr std::partial_ordering operator<=>(const Iterator &other) const
                requires(std::random_access_iterator<std::ranges::iterator_t<R>> && ...)
            {
                auto distance = *this - other;
                if (distance == 0) {
                    return std::partial_ordering::equivalent;
                }

                if (distance < 0) {
                    return std::partial_ordering::less;
                }

                return std::partial_ordering::greater;
            }

            constexpr Iterator &operator--()
                requires(std::bidirectional_iterator<std::ranges::iterator_t<R>> && ...)
            {
                visit_index(Prev{this}, it);
                return *this;
            }

            constexpr auto operator--(int) {
                if constexpr ((std::ranges::forward_range<R> && ...)) {
                    auto tmp = *this;
                    --*this;
                    return tmp;
                } else {
                    --*this;
                }
            }

            constexpr Iterator operator+(difference_type n) const
                requires(std::random_access_iterator<std::ranges::iterator_t<R>> && ...)
            {
                Iterator tmp(*this);
                tmp += n;
                return tmp;
            }

            friend Iterator operator+(difference_type n, const Iterator &it)
                requires(std::random_access_iterator<std::ranges::iterator_t<R>> && ...)
            {
                return it + n;
            }

            constexpr Iterator &operator+=(difference_type n)
                requires(std::random_access_iterator<std::ranges::iterator_t<R>> && ...)
            {
                if (n > 0) {
                    visit_index<void>(AdvanceForward{this, n}, it);
                } else if (n < 0) {
                    visit_index<void>(AdvanceReverse{this, n}, it);
                }
                return *this;
            }

            constexpr Iterator operator-(difference_type n) const
                requires(std::random_access_iterator<std::ranges::iterator_t<R>> && ...)
            {
                Iterator tmp(*this);
                tmp -= n;
                return tmp;
            }

            constexpr difference_type operator-(const Iterator &other) const
                requires(std::sized_sentinel_for<std::ranges::iterator_t<R>, std::ranges::iterator_t<R>> && ...)
            {
                if (it.index() <= other.it.index()) {
                    return -Iterator::distance_to(std::integral_constant<size_t, 0>{}, *this, other);
                }

                return Iterator::distance_to(std::integral_constant<size_t, 0>{}, other, *this);
            }

            constexpr Iterator &operator-=(difference_type n)
                requires(std::random_access_iterator<std::ranges::iterator_t<R>> && ...)
            {
                return *this += -n;
            }

            constexpr Reference operator[](difference_type n) const
                requires(std::random_access_iterator<std::ranges::iterator_t<R>> && ...)
            {
                return *(*this + n);
            }
        };

      public:
        /**
         * @brief Default constructor for the ConcatView class.
         *
         * Creates a ConcatView object with default initialization.
         * This constructor is marked as constexpr, allowing for compile-time construction when used with constant
         * expressions.
         *
         * @return A constexpr ConcatView object.
         */
        constexpr ConcatView() = default;

        /**
         * @brief Constructor for the ConcatView class accepting multiple ranges.
         *
         * This constructor initializes the ConcatView object with the provided ranges,
         * allowing multiple ranges to be concatenated. It utilizes perfect forwarding
         * to efficiently transfer the given ranges into the internal storage.
         *
         * @param ranges Variadic parameter pack representing the ranges to be concatenated.
         */
        constexpr explicit ConcatView(R... ranges) : ranges(std::move(ranges)...) {
        }

        /**
         * @brief Returns an iterator to the beginning of the ConcatView.
         *
         * Constructs and returns an iterator pointing to the first element of the ConcatView.
         * This function is marked constexpr, allowing usage in constant expressions where eligible.
         *
         * @return An iterator to the beginning of the ConcatView.
         */
        constexpr auto begin() {
            return Iterator<(SimpleView<R> && ...)>(*this, BeginTag{});
        }

        /**
         * @brief Returns an iterator to the beginning of the ConcatView.
         *
         * Constructs and returns an iterator pointing to the first element of the ConcatView.
         * This function is marked constexpr, allowing usage in constant expressions where eligible.
         *
         * @return An iterator to the beginning of the ConcatView.
         */
        constexpr Iterator<true> begin() const
            requires(std::ranges::range<const R> && ...)
        {
            return Iterator<true>(*this, BeginTag{});
        }

        /**
         * @brief Returns the end iterator or sentinel for the range.
         *
         * This method determines whether to return an iterator or a sentinel
         * based on the range properties. If all ranges are common ranges, it
         * returns an iterator; otherwise, it returns a sentinel.
         *
         * @return A compile-time constant result that is either an iterator or a sentinel,
         *         depending on the properties of the range.
         */
        constexpr auto end() {
            if constexpr ((std::ranges::common_range<R> && ...)) {
                return Iterator<(SimpleView<R> && ...)>(*this, EndTag{});
            } else {
                return Sentinel<(SimpleView<R> && ...)>(*this, EndTag{});
            }
        }

        /**
         * @brief Returns the end iterator or sentinel for the range.
         *
         * This method determines whether to return an iterator or a sentinel
         * based on the range properties. If all ranges are common ranges, it
         * returns an iterator; otherwise, it returns a sentinel.
         *
         * @return A compile-time constant result that is either an iterator or a sentinel,
         *         depending on the properties of the range.
         */
        constexpr auto end() const
            requires(std::ranges::range<const R> && ...)
        {
            if constexpr ((std::ranges::common_range<R> && ...)) {
                return Iterator<true>(*this, EndTag{});
            } else {
                return Sentinel<true>(*this, EndTag{});
            }
        }

        /**
         * @brief Computes the total size of all ranges.
         *
         * This method calculates the combined size of all the ranges stored in the object.
         * It requires that all ranges satisfy the `std::ranges::sized_range` concept.
         * The computation is performed using a fold expression over the sizes of the individual ranges.
         *
         * @return The total size of all ranges as a `constexpr` value.
         */
        constexpr size_t size() const
            requires(std::ranges::sized_range<R> && ...)
        {
            return std::apply([](auto &...r) { return (std::ranges::size(r) + ...); }, ranges);
        }
    };

    /**
     * Constructs a ConcatView object from one or more ranges.
     * Combines multiple ranges into a single concatenated, viewable range.
     *
     * @param ...rng A pack of ranges to be concatenated. Each range is passed by universal reference.
     * @return A ConcatView object that represents the concatenated view of all the input ranges.
     */
    template <typename... Rng>
    ConcatView(Rng &&...) -> ConcatView<std::ranges::views::all_t<Rng>...>;

    namespace views {
        /**
         * @brief Invokes the creation of a ConcatView instance from multiple input ranges.
         *
         * This callable struct provides functionality to construct a ConcatView object
         * from a variadic number of input ranges. The input ranges must satisfy the
         * requirements of `std::ranges::input_range` and `Concatable`, and a ConcatView
         * must be constructible from these ranges. The operator ensures that there is
         * at least one input range provided.
         *
         * @tparam R Variadic template parameter representing the types of input ranges.
         *           Each type must satisfy `std::ranges::input_range`.
         *
         * @param ranges The input ranges to be concatenated through the ConcatView.
         *               This parameter pack is forwarded to ensure proper value category handling.
         *
         * @return A ConcatView constructed with the provided ranges, where each range
         *         is wrapped in `std::ranges::views::all`.
         */
        struct ConcatInvoker {

            /**
             * @brief Call operator for creating a ConcatView with the given ranges.
             *
             * This operator applies the ConcatView to a collection of ranges, forwarding each range
             * and ensuring compatibility with the std::ranges::views::all adapter. The result is
             * a concatenated view of the provided ranges.
             *
             * @param ranges The ranges to be concatenated. These ranges are forwarded and processed by the
             * std::ranges::views::all adapter.
             * @return A ConcatView object representing the concatenated view of the input ranges.
             */
            template <std::ranges::input_range... R>
                requires std::constructible_from<ConcatView<std::ranges::views::all_t<R>...>,
                                                 std::ranges::views::all_t<R>...> &&
                         (sizeof...(R) > 0) && Concatable<R...>
            constexpr auto operator()(R &&...ranges) const {
                return ConcatView(std::ranges::views::all(std::forward<R>(ranges))...);
            }
        };

        /**
         * @brief A constexpr instance of ConcatInvoker.
         *
         * Represents a compile-time constant invoker for the concat view operation.
         * This allows for seamless chaining of range concatenation operations
         * within the retro::ranges::views namespace.
         */
        RETROLIB_EXPORT constexpr ConcatInvoker concat;
    } // namespace views
} // namespace retro::ranges
