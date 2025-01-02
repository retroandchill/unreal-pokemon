/**
 * @file JoinWith.h
 * @brief Allows the user to join with another view in-between.
 *
 * @author Retro & Chill
 * https://github.com/retroandchill
 */
#pragma once

#include "RetroLib/Functional/ExtensionMethods.h"
#include "RetroLib/Ranges/Concepts/Concatable.h"
#include "RetroLib/Ranges/RangeBasics.h"

#if !RETROLIB_WITH_MODULES
#include <variant>
#endif

#ifndef RETROLIB_EXPORT
#define RETROLIB_EXPORT
#endif

namespace Retro::Ranges {
    template <typename I>
    struct TStoreInner {
        TNonPropagatingCache<std::remove_cv_t<I>> Inner;

        template <typename O>
        constexpr auto &&UpdateInner(O &&It) {
            return Inner.EmplaceDeref(It);
        }

        constexpr I &GetInner(auto) {
            return *Inner;
        }
    };

    struct FPassThroughInner {
        template <typename O>
        static constexpr auto &&UpdateInner(O &&It) noexcept(noexcept(*std::forward<O>(It))) {
            return *std::forward<O>(It);
        }

        template <typename O>
        static constexpr decltype(auto) GetInner(O &&It) {
            return *std::forward<O>(It);
        }
    };

    template <std::ranges::input_range R>
    using TJoinViewInner = std::conditional_t<!std::is_reference_v<std::ranges::range_reference_t<R>>,
                                             TStoreInner<std::ranges::range_reference_t<R>>, FPassThroughInner>;

    /**
     * @brief A view that flattens a range of ranges, inserting a separator element between the inner ranges.
     *
     * The `JoinWithView` class provides a range adaptor that enables the flattening of a range of ranges,
     * inserting a specified "contraction" sequence or element between each inner range. This facilitates
     * combining nested ranges into a single range while interjecting a custom separator.
     *
     * This class is part of the ranges library and adheres to the view interface, allowing it to
     * participate in range-based algorithms and be lazily evaluated.
     *
     * @tparam R The type of the outer range. Must be a range and its reference must itself be a range.
     * @tparam P The type of the "contraction" separator element or range.
     */
    RETROLIB_EXPORT template <std::ranges::input_range R, std::ranges::forward_range P>
        requires std::ranges::view<R> && std::ranges::input_range<std::ranges::range_reference_t<R>> &&
                     std::ranges::view<P> && Concatable<std::ranges::range_reference_t<R>, P>
    class TJoinWithView : public std::ranges::view_interface<TJoinWithView<R, P>>, private TJoinViewInner<R> {
        using OuterType = std::ranges::views::all_t<R>;
        using InnerType = std::ranges::views::all_t<std::ranges::range_reference_t<R>>;
        using SizeType = std::common_type_t<std::ranges::range_difference_t<InnerType>, std::ranges::range_difference_t<P>>;

        class FIterator {
          public:
            using value_type = std::common_type_t<std::ranges::range_value_t<InnerType>, std::ranges::range_value_t<P>>;
            using reference =
                std::common_reference_t<std::ranges::range_reference_t<InnerType>, std::ranges::range_reference_t<P>>;
            using rvalue_reference = std::common_reference_t<std::ranges::range_rvalue_reference_t<InnerType>,
                                                             std::ranges::range_rvalue_reference_t<P>>;
            using single_pass = std::true_type;
            using difference_type = SizeType;

            constexpr FIterator() = default;

            constexpr explicit FIterator(TJoinWithView &Range)
                : Range(&Range), OuterIt(std::ranges::begin(Range.Outer)) {
                if (OuterIt != std::ranges::end(Range.Outer)) {
                    auto &&Inner = Range.UpdateInner(OuterIt);
                    Current.template emplace<1>(std::ranges::begin(Inner));
                    Satisfy();
                }
            }

            constexpr bool operator==(std::default_sentinel_t) const {
                return OuterIt == std::ranges::end(Range->Outer);
            }

            constexpr FIterator &operator++() {
                if (Current.index() == 0) {
                    auto &It = std::get<0>(Current);
                    RETROLIB_ASSERT(It != std::ranges::end(Range->Contraction));
                    ++It;
                } else {
                    auto &It = std::get<1>(Current);
#ifndef NDEBUG
                    auto &&Inner = Range->GetInner(OuterIt);
                    RETROLIB_ASSERT(It != std::ranges::end(Inner));
#endif
                    ++It;
                }
                Satisfy();
                return *this;
            }

            constexpr void operator++(int) {
                ++*this;
            }

            constexpr reference operator*() const & {
                if (Current.index() == 0) {
                    return *std::get<0>(Current);
                }

                return *std::get<1>(Current);
            }

            constexpr rvalue_reference operator*() && {
                if (Current.index() == 0) {
                    return std::move(*std::get<0>(Current));
                }

                return std::move(*std::get<1>(Current));
            }

          private:
            void Satisfy() {
                while (true) {
                    if (Current.index() == 0) {
                        if (std::get<0>(Current) != std::ranges::end(Range->Contraction)) {
                            break;
                        }

                        auto &&Inner = Range->UpdateInner(OuterIt);
                        Current.template emplace<1>(std::ranges::begin(Inner));
                    } else {
                        if (auto &&Inner = Range->GetInner(OuterIt); std::get<1>(Current) != std::ranges::end(Inner)) {
                            break;
                        }

                        ++OuterIt;
                        if (OuterIt == std::ranges::end(Range->Outer)) {
                            break;
                        }

                        Current.template emplace<0>(std::ranges::begin(Range->Contraction));
                    }
                }
            }

            TJoinWithView *Range = nullptr;
            std::ranges::iterator_t<R> OuterIt;
            std::variant<std::ranges::iterator_t<P>, std::ranges::iterator_t<InnerType>> Current;
        };

      public:
        /**
         * @brief Default constructor for the JoinWithView class.
         *
         * This constructor initializes an instance of the class with default values.
         *
         * @return A default-constructed instance of JoinWithView.
         */
        constexpr TJoinWithView() = default;

        /**
         * @brief Constructs a JoinWithView instance with specified outer range and contraction functor.
         *
         * This constructor initializes the JoinWithView with an outer range and a contraction
         * functor, moving the provided arguments into the instance.
         *
         * @param Outer The outer range used for joining.
         * @param Contraction The functor or predicate used for contraction logic.
         *
         * @return A constructed instance of JoinWithView with the specified values.
         */
        constexpr TJoinWithView(R Outer, P Contraction) : Outer(std::move(Outer)), Contraction(std::move(Contraction)) {
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
            return Outer;
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
            return std::move(Outer);
        }

        /**
         * @brief Retrieves an iterator to the beginning of the container.
         *
         * This method provides an iterator that points to the first element
         * of the container. The returned iterator allows iteration over the
         * container starting from its first element.
         *
         * @return An iterator positioned at the beginning of the container.
         */
        constexpr FIterator begin() {
            return FIterator(*this);
        }

        /**
         * @brief Returns the default sentinel for the end of a range.
         *
         * This function provides a default sentinel to represent the end of a range.
         *
         * @return The default sentinel of type std::default_sentinel_t.
         */
        constexpr std::default_sentinel_t end() {
            return std::default_sentinel;
        }

        /**
         * @brief Computes the size of the range by considering the sizes of outer, contraction, and inner ranges.
         *
         * This method calculates the total size of the composite range by iterating over the outer range, factoring in
         * the contraction size, and summing up the sizes of each element within the outer range.
         *
         * @return The computed total size of the range as `SizeType`.
         */
        constexpr SizeType size() const
            requires std::ranges::sized_range<OuterType> && std::ranges::sized_range<InnerType> &&
                     std::ranges::sized_range<P> && std::ranges::forward_range<OuterType>
        {
            SizeType size = (std::ranges::size(Outer) - 1) * std::ranges::size(Contraction);
            for (auto &&it : Outer) {
                size += std::ranges::size(std::forward<decltype(it)>(it));
            }
            return size;
        }

      private:
        OuterType Outer;
        std::ranges::views::all_t<P> Contraction;
    };

    /**
     * Creates a JoinWithView object from the given range and pattern.
     *
     * This method takes two input arguments: a range (R) and a pattern (P),
     * and constructs a JoinWithView that represents a view that joins a range
     * of ranges (R) together using a specified pattern (P).
     *
     * @param R The outer range to operate on, where each element is a subrange.
     *          It is transformed into a view using std::ranges::views::all_t.
     * @param P The pattern to intersperse between the joined elements of the outer range.
     *          It is also transformed into a view using std::ranges::views::all_t.
     * @return A JoinWithView object that joins together the subranges in the outer range
     *         with the specified pattern.
     */
    template <std::ranges::input_range R, std::ranges::forward_range P>
    TJoinWithView(R &&, P &&) -> TJoinWithView<std::ranges::views::all_t<R>, std::ranges::views::all_t<P>>;

    namespace Views {
        struct FJoinWithInvoker {
            /**
             * @brief Creates a JoinWithView by joining a range with a contraction element.
             *
             * This function takes a range and a contraction element, forwarding them to create
             * a JoinWithView. The function ensures that the range and contraction elements are
             * correctly transformed into views before constructing the resultant view.
             *
             * @param Range The input range to be joined.
             * @param Contraction The element used as the separator between elements of the range.
             * @return A JoinWithView representing the joined range with the specified contraction.
             */
            template <std::ranges::input_range R, std::ranges::forward_range P>
                requires std::ranges::viewable_range<R> &&
                         std::ranges::input_range<std::ranges::range_reference_t<R>> &&
                         std::ranges::viewable_range<P> && Concatable<std::ranges::range_reference_t<R>, P>
            constexpr auto operator()(R &&Range, P &&Contraction) const {
                return TJoinWithView(std::ranges::views::all(std::forward<R>(Range)),
                                    std::ranges::views::all(std::forward<P>(Contraction)));
            }

            /**
             * @brief Joins a range with a single contraction value.
             *
             * This function forwards a range and a contraction value to create a new
             * range where each element in the input range is joined with the provided
             * contraction using `std::ranges::views::single`.
             *
             * @param Range The input range to be joined.
             * @param Contraction The value to be used for joining elements of the range.
             * @return A new range resulting from joining the input range with the contraction value.
             */
            template <std::ranges::input_range R, typename P>
                requires std::ranges::viewable_range<R> &&
                         std::ranges::input_range<std::ranges::range_reference_t<R>> &&
                         std::convertible_to<P, std::ranges::range_value_t<std::ranges::range_reference_t<R>>>
            constexpr auto operator()(R &&Range, P &&Contraction) const {
                return (*this)(std::forward<R>(Range), std::ranges::views::single(std::forward<P>(Contraction)));
            }

            template <std::ranges::input_range R, typename P>
                requires std::ranges::viewable_range<R> &&
                         std::ranges::input_range<std::ranges::range_reference_t<R>> &&
                         std::convertible_to<P, std::ranges::range_value_t<std::ranges::range_reference_t<R>>> &&
                         std::constructible_from<std::basic_string_view<P>, const P *>
            constexpr auto operator()(R &&Range, const P *Contraction) const {
                return (*this)(std::forward<R>(Range), std::basic_string_view<P>(Contraction));
            }
        };

        constexpr FJoinWithInvoker JoinWithFunction;

        /**
         * @brief Combines elements of a range with a specified separator.
         *
         * This function creates a range adapter that combines elements of a range,
         * separated by the specified contraction. The separator can be any type
         * that is forwarded to the final adapter.
         *
         * @param contraction The separator or delimiter used to join elements of the range.
         * @return A range adapter that joins elements of the range with the provided separator.
         */
        RETROLIB_EXPORT constexpr auto JoinWith = ExtensionMethod<JoinWithFunction>;
    } // namespace views
} // namespace Retro::Ranges
