/**
 * @file Concat.h
 * @brief View for concatenating two or more dissimilar ranges.
 *
 * @author Retro & Chill
 * https://github.com/retroandchill
 */
#pragma once

#include "RetroLib/Concepts/Iterators.h"
#include "RetroLib/Ranges/Concepts/Concatable.h"
#include "RetroLib/Concepts/ParameterPacks.h"
#include "RetroLib/Ranges/RangeBasics.h"
#include "RetroLib/Utils/Unreachable.h"
#include "RetroLib/Utils/Variant.h"

#if !RETROLIB_WITH_MODULES
#include <tuple>
#include <variant>
#endif

#ifndef RETROLIB_EXPORT
#define RETROLIB_EXPORT
#endif

namespace Retro::Ranges {

    /**
     * @class TConcatView
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
    class TConcatView : public std::ranges::view_interface<TConcatView<R...>> {
        using DifferenceType = std::common_type_t<std::ranges::range_difference_t<R>...>;
        static constexpr size_t RangesSize = sizeof...(R);
        std::tuple<R...> Ranges;

        template <bool IsConst>
        struct TIterator;

        template <bool IsConst>
        struct TSentinel {
          private:
            friend struct TSentinel<!IsConst>;
            friend struct TIterator<IsConst>;
            template <typename T>
            using ConstifyIf = std::conditional_t<IsConst, std::add_const_t<T>, T>;
            using ConcatViewType = ConstifyIf<TConcatView>;
            std::ranges::sentinel_t<ConstifyIf<TLastInPack<R...>>> EndElement;

          public:
            constexpr TSentinel() = default;
            explicit constexpr TSentinel(ConcatViewType &View, FEndTag)
                : EndElement(std::ranges::end(std::get<RangesSize - 1>(View.ranges))) {
            }

            template <bool Other>
                requires IsConst && (!Other)
            explicit constexpr TSentinel(TSentinel<Other> Sen) : EndElement(std::move(Sen.EndElement)) {
            }
        };

        template <bool IsConst>
        struct TIterator {
            using difference_type = std::common_type_t<std::ranges::range_difference_t<R>...>;
            using value_type = TConcatValue<R...>;

          private:
            friend struct TIterator<!IsConst>;
            template <typename T>
            using ConstifyIf = std::conditional_t<IsConst, std::add_const_t<T>, T>;
            using ConcatViewType = ConstifyIf<TConcatView>;
            ConcatViewType *View;
            std::variant<std::ranges::iterator_t<ConstifyIf<R>>...> It;

            template <size_t N>
            constexpr void Satisfy() {
                RETROLIB_ASSERT(It.index() == N);
                if constexpr (N < RangesSize - 1) {
                    if (std::get<N>(It) == std::ranges::end(std::get<N>(View->Ranges))) {
                        It.template emplace<N + 1>(std::ranges::begin(std::get<N + 1>(View->Ranges)));
                        Satisfy<N + 1>();
                    }
                }
            }

            struct Next {
                TIterator *Pos;
                template <std::input_iterator I, size_t N>
                constexpr void operator()(TIndexedElement<I, N> It) const {
                    RETROLIB_ASSERT(It.Get() != std::ranges::end(std::get<N>(Pos->View->Ranges)));
                    ++It.Get();
                    Pos->Satisfy<N>();
                }
            };

            struct Prev {
                TIterator *Pos;

                template <std::bidirectional_iterator I>
                constexpr void operator()(TIndexedElement<I, 0> It) const {
                    RETROLIB_ASSERT(It.Get() != std::ranges::begin(std::get<0>(Pos->View->Ranges)));
                    --It.Get();
                }

                template <std::bidirectional_iterator I, size_t N>
                    requires(N != 0)
                constexpr void operator()(TIndexedElement<I, N> It) const {
                    if (It.Get() == std::ranges::begin(std::get<N>(Pos->View->Ranges))) {
                        auto &&Rng = std::get<N - 1>(Pos->View->Ranges);
                        Pos->It.template emplace<N - 1>(
                            std::ranges::next(std::ranges::begin(Rng), std::ranges::end(Rng)));
                        VisitIndex(*this, Pos->It);
                    } else {
                        --It.Get();
                    }
                }
            };

            struct AdvanceForward {
                TIterator *Pos;
                difference_type Index;

                template <std::random_access_iterator I>
                constexpr void operator()(TIndexedElement<I, RangesSize - 1> It) const {
                    std::ranges::advance(It.Get(), Index);
                }

                template <std::random_access_iterator I, size_t N>
                constexpr void operator()(TIndexedElement<I, N> It) const {
                    auto Last = std::ranges::end(std::get<N>(Pos->View->Ranges));
                    auto Rest = std::ranges::advance(It.Get(), Index, std::move(Last));
                    Pos->Satisfy<N>();

                    if (Rest != 0) {
                        VisitIndex<void>(AdvanceForward{Pos, Rest}, Pos->It);
                    }
                }
            };

            struct AdvanceReverse {
                TIterator *Pos;
                difference_type Index;

                template <std::random_access_iterator I>
                constexpr void operator()(TIndexedElement<I, 0> It) const {
                    std::ranges::advance(It.Get(), Index);
                }

                template <std::random_access_iterator I, std::size_t N>
                constexpr void operator()(TIndexedElement<I, N> It) const {
                    auto First = std::ranges::begin(std::get<N>(Pos->View->Ranges));
                    if (It.Get() == First) {
                        auto &&Rng = std::get<N - 1>(Pos->View->Ranges);
                        Pos->It.template emplace<N - 1>(
                            std::ranges::next(std::ranges::begin(Rng), std::ranges::end(Rng)));
                        VisitIndex(*this, Pos->It);
                    } else {
                        auto Rest = std::ranges::advance(It.Get(), Index, std::move(First));
                        if (Rest != 0) {
                            VisitIndex<void>(AdvanceReverse{Pos, Rest}, Pos->It);
                        }
                    }
                }
            };

            [[noreturn]] static difference_type DistanceTo(std::integral_constant<size_t, RangesSize>,
                                                            const TIterator &, const TIterator &) {
                RETROLIB_ASSERT(false);
                Unreachable();
            }

            template <size_t N>
                requires(N < RangesSize)
            static difference_type DistanceTo(std::integral_constant<size_t, N>, const TIterator &From,
                                               const TIterator &To) {
                if (From.It.index() > N) {
                    return TIterator::DistanceTo(std::integral_constant<size_t, N + 1>{}, From, To);
                }

                if (From.It.index() == N) {
                    if (To.It.index() == N) {
                        return std::ranges::distance(std::get<N>(From.It), std::get<N>(To.It));
                    }

                    return std::ranges::distance(std::get<N>(From.It),
                                                 std::ranges::end(std::get<N>(From.View->Ranges))) +
                           TIterator::DistanceTo(std::integral_constant<size_t, N + 1>{}, From, To);
                }
                if (From.It.index() < N && To.It.index() > N)
                    return std::ranges::distance(std::get<N>(From.View->Ranges)) +
                           TIterator::DistanceTo(std::integral_constant<size_t, N + 1>{}, From, To);

                RETROLIB_ASSERT(To.It.index() == N);
                return std::ranges::distance(std::ranges::begin(std::get<N>(From.View->Ranges)), std::get<N>(To.It));
            }

          public:
            using Reference = std::common_reference_t<std::ranges::range_reference_t<ConstifyIf<R>>...>;
            using single_pass = std::bool_constant<(SinglePassIterator<std::ranges::iterator_t<R>> || ...)>;

            constexpr TIterator() = default;

            constexpr TIterator(ConcatViewType &View, FBeginTag)
                : View(&View), It(std::in_place_index<0>, std::ranges::begin(std::get<0>(View.Ranges))) {
                Satisfy<0>();
            }

            constexpr TIterator(ConcatViewType &View, FEndTag)
                : View(&View),
                  It(std::in_place_index<RangesSize - 1>, std::ranges::end(std::get<RangesSize - 1>(View.Ranges))) {
            }

            template <bool Other>
                requires IsConst && (!Other)
            explicit constexpr TIterator(TIterator<Other> OtherIn) : View(OtherIn.View), It(std::move(OtherIn.It)) {
            }

            constexpr Reference operator*() const {
                return std::visit<Reference>([]<typename T>(T &&It) -> decltype(auto) { return *std::forward<T>(It); },
                                             It);
            }

            constexpr TIterator &operator++() {
                VisitIndex(Next{this}, It);
                return *this;
            }

            constexpr auto operator++(int) {
                if constexpr ((std::ranges::forward_range<R> && ...)) {
                    auto Tmp = *this;
                    ++*this;
                    return Tmp;
                } else {
                    ++*this;
                }
            }

            constexpr bool operator==(const TIterator &pos) const
                requires std::equality_comparable<std::variant<std::ranges::iterator_t<ConstifyIf<R>>...>>
            {
                return It == pos.It;
            }

            constexpr bool operator==(const TSentinel<IsConst> &Post) const {
                return It.index() == RangesSize - 1 && std::get<RangesSize - 1>(View->it) == Post.EndElement;
            }

            constexpr std::partial_ordering operator<=>(const TIterator &Other) const
                requires(std::random_access_iterator<std::ranges::iterator_t<R>> && ...)
            {
                auto distance = *this - Other;
                if (distance == 0) {
                    return std::partial_ordering::equivalent;
                }

                if (distance < 0) {
                    return std::partial_ordering::less;
                }

                return std::partial_ordering::greater;
            }

            constexpr TIterator &operator--()
                requires(std::bidirectional_iterator<std::ranges::iterator_t<R>> && ...)
            {
                VisitIndex(Prev{this}, It);
                return *this;
            }

            constexpr auto operator--(int) {
                if constexpr ((std::ranges::forward_range<R> && ...)) {
                    auto Tmp = *this;
                    --*this;
                    return Tmp;
                } else {
                    --*this;
                }
            }

            constexpr TIterator operator+(difference_type N) const
                requires(std::random_access_iterator<std::ranges::iterator_t<R>> && ...)
            {
                TIterator Tmp(*this);
                Tmp += N;
                return Tmp;
            }

            friend TIterator operator+(difference_type N, const TIterator &It)
                requires(std::random_access_iterator<std::ranges::iterator_t<R>> && ...)
            {
                return It + N;
            }

            constexpr TIterator &operator+=(difference_type N)
                requires(std::random_access_iterator<std::ranges::iterator_t<R>> && ...)
            {
                if (N > 0) {
                    VisitIndex<void>(AdvanceForward{this, N}, It);
                } else if (N < 0) {
                    VisitIndex<void>(AdvanceReverse{this, N}, It);
                }
                return *this;
            }

            constexpr TIterator operator-(difference_type N) const
                requires(std::random_access_iterator<std::ranges::iterator_t<R>> && ...)
            {
                TIterator Tmp(*this);
                Tmp -= N;
                return Tmp;
            }

            constexpr difference_type operator-(const TIterator &Other) const
                requires(std::sized_sentinel_for<std::ranges::iterator_t<R>, std::ranges::iterator_t<R>> && ...)
            {
                if (It.index() <= Other.It.index()) {
                    return -TIterator::DistanceTo(std::integral_constant<size_t, 0>{}, *this, Other);
                }

                return TIterator::DistanceTo(std::integral_constant<size_t, 0>{}, Other, *this);
            }

            constexpr TIterator &operator-=(difference_type N)
                requires(std::random_access_iterator<std::ranges::iterator_t<R>> && ...)
            {
                return *this += -N;
            }

            constexpr Reference operator[](difference_type N) const
                requires(std::random_access_iterator<std::ranges::iterator_t<R>> && ...)
            {
                return *(*this + N);
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
        constexpr TConcatView() = default;

        /**
         * @brief Constructor for the ConcatView class accepting multiple ranges.
         *
         * This constructor initializes the ConcatView object with the provided ranges,
         * allowing multiple ranges to be concatenated. It utilizes perfect forwarding
         * to efficiently transfer the given ranges into the internal storage.
         *
         * @param Ranges Variadic parameter pack representing the ranges to be concatenated.
         */
        constexpr explicit TConcatView(R... Ranges) : Ranges(std::move(Ranges)...) {
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
            return TIterator<(SimpleView<R> && ...)>(*this, FBeginTag{});
        }

        /**
         * @brief Returns an iterator to the beginning of the ConcatView.
         *
         * Constructs and returns an iterator pointing to the first element of the ConcatView.
         * This function is marked constexpr, allowing usage in constant expressions where eligible.
         *
         * @return An iterator to the beginning of the ConcatView.
         */
        constexpr TIterator<true> begin() const
            requires(std::ranges::range<const R> && ...)
        {
            return TIterator<true>(*this, FBeginTag{});
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
                return TIterator<(SimpleView<R> && ...)>(*this, FEndTag{});
            } else {
                return TSentinel<(SimpleView<R> && ...)>(*this, FEndTag{});
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
                return TIterator<true>(*this, FEndTag{});
            } else {
                return TSentinel<true>(*this, FEndTag{});
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
            return std::apply([](auto &...r) { return (std::ranges::size(r) + ...); }, Ranges);
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
    TConcatView(Rng &&...) -> TConcatView<std::ranges::views::all_t<Rng>...>;

    namespace Views {
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
        struct FConcatInvoker {

            /**
             * @brief Call operator for creating a ConcatView with the given ranges.
             *
             * This operator applies the ConcatView to a collection of ranges, forwarding each range
             * and ensuring compatibility with the std::ranges::views::all adapter. The result is
             * a concatenated view of the provided ranges.
             *
             * @param Ranges The ranges to be concatenated. These ranges are forwarded and processed by the
             * std::ranges::views::all adapter.
             * @return A ConcatView object representing the concatenated view of the input ranges.
             */
            template <std::ranges::input_range... R>
                requires std::constructible_from<TConcatView<std::ranges::views::all_t<R>...>,
                                                 std::ranges::views::all_t<R>...> &&
                         (sizeof...(R) > 0) && Concatable<R...>
            constexpr auto operator()(R &&...Ranges) const {
                return TConcatView(std::ranges::views::all(std::forward<R>(Ranges))...);
            }
        };

        /**
         * @brief A constexpr instance of ConcatInvoker.
         *
         * Represents a compile-time constant invoker for the concat view operation.
         * This allows for seamless chaining of range concatenation operations
         * within the retro::ranges::views namespace.
         */
        RETROLIB_EXPORT constexpr FConcatInvoker Concat;
    } // namespace views
} // namespace retro::ranges
