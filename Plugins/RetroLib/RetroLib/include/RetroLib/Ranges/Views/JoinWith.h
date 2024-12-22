/**
 * @file JoinWith.h
 * @brief Allows the user to join with another view in-between.
 *
 * @author Retro & Chill
 * https://github.com/retroandchill
 */
#pragma once

#if !RETROLIB_WITH_MODULES
#include "RetroLib/Functional/ExtensionMethods.h"
#include "RetroLib/Ranges/Concepts/Concatable.h"
#include "RetroLib/Ranges/RangeBasics.h"

#include <variant>
#endif

#ifndef RETROLIB_EXPORT
#define RETROLIB_EXPORT
#endif

namespace retro::ranges {
    template <typename I>
    struct StoreInner {
        NonPropagatingCache<std::remove_cv_t<I>> inner;

        template <typename O>
        constexpr auto &&update_inner(O &&it) {
            return inner.emplace_deref(it);
        }

        constexpr I &get_inner(auto) {
            return *inner;
        }
    };

    struct PassThroughInner {
        template <typename O>
        static constexpr auto &&update_inner(O &&it) noexcept(noexcept(*std::forward<O>(it))) {
            return *std::forward<O>(it);
        }

        template <typename O>
        static constexpr decltype(auto) get_inner(O &&it) {
            return *std::forward<O>(it);
        }
    };

    template <std::ranges::input_range R>
    using JoinViewInner = std::conditional_t<!std::is_reference_v<std::ranges::range_reference_t<R>>,
                                             StoreInner<std::ranges::range_reference_t<R>>, PassThroughInner>;

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
    class JoinWithView : public std::ranges::view_interface<JoinWithView<R, P>>, private JoinViewInner<R> {
        using Outer = std::ranges::views::all_t<R>;
        using Inner = std::ranges::views::all_t<std::ranges::range_reference_t<R>>;
        using SizeType = std::common_type_t<std::ranges::range_difference_t<Inner>, std::ranges::range_difference_t<P>>;

        class Iterator {
          public:
            using value_type = std::common_type_t<std::ranges::range_value_t<Inner>, std::ranges::range_value_t<P>>;
            using reference =
                std::common_reference_t<std::ranges::range_reference_t<Inner>, std::ranges::range_reference_t<P>>;
            using rvalue_reference = std::common_reference_t<std::ranges::range_rvalue_reference_t<Inner>,
                                                             std::ranges::range_rvalue_reference_t<P>>;
            using single_pass = std::true_type;
            using difference_type = SizeType;

            constexpr Iterator() = default;

            constexpr explicit Iterator(JoinWithView &range)
                : range(&range), outer_it(std::ranges::begin(range.outer)) {
                if (outer_it != std::ranges::end(range.outer)) {
                    auto &&inner = range.update_inner(outer_it);
                    current.template emplace<1>(std::ranges::begin(inner));
                    satisfy();
                }
            }

            constexpr bool operator==(std::default_sentinel_t) const {
                return outer_it == std::ranges::end(range->outer);
            }

            constexpr Iterator &operator++() {
                if (current.index() == 0) {
                    auto &it = std::get<0>(current);
                    RETROLIB_ASSERT(it != std::ranges::end(range->contraction));
                    ++it;
                } else {
                    auto &it = std::get<1>(current);
#ifndef NDEBUG
                    auto &&inner = range->get_inner(outer_it);
                    RETROLIB_ASSERT(it != std::ranges::end(inner));
#endif
                    ++it;
                }
                satisfy();
                return *this;
            }

            constexpr void operator++(int) {
                ++*this;
            }

            constexpr reference operator*() const & {
                if (current.index() == 0) {
                    return *std::get<0>(current);
                }

                return *std::get<1>(current);
            }

            constexpr rvalue_reference operator*() && {
                if (current.index() == 0) {
                    return std::move(*std::get<0>(current));
                }

                return std::move(*std::get<1>(current));
            }

          private:
            void satisfy() {
                while (true) {
                    if (current.index() == 0) {
                        if (std::get<0>(current) != std::ranges::end(range->contraction)) {
                            break;
                        }

                        auto &&inner = range->update_inner(outer_it);
                        current.template emplace<1>(std::ranges::begin(inner));
                    } else {
                        auto &&inner = range->get_inner(outer_it);
                        if (std::get<1>(current) != std::ranges::end(inner)) {
                            break;
                        }

                        ++outer_it;
                        if (outer_it == std::ranges::end(range->outer)) {
                            break;
                        }

                        current.template emplace<0>(std::ranges::begin(range->contraction));
                    }
                }
            }

            JoinWithView *range = nullptr;
            std::ranges::iterator_t<R> outer_it;
            std::variant<std::ranges::iterator_t<P>, std::ranges::iterator_t<Inner>> current;
        };

      public:
        /**
         * @brief Default constructor for the JoinWithView class.
         *
         * This constructor initializes an instance of the class with default values.
         *
         * @return A default-constructed instance of JoinWithView.
         */
        constexpr JoinWithView() = default;

        /**
         * @brief Constructs a JoinWithView instance with specified outer range and contraction functor.
         *
         * This constructor initializes the JoinWithView with an outer range and a contraction
         * functor, moving the provided arguments into the instance.
         *
         * @param outer The outer range used for joining.
         * @param contraction The functor or predicate used for contraction logic.
         *
         * @return A constructed instance of JoinWithView with the specified values.
         */
        constexpr JoinWithView(R outer, P contraction) : outer(std::move(outer)), contraction(std::move(contraction)) {
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
        constexpr Iterator begin() {
            return Iterator(*this);
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
            requires std::ranges::sized_range<Outer> && std::ranges::sized_range<Inner> &&
                     std::ranges::sized_range<P> && std::ranges::forward_range<Outer>
        {
            SizeType size = (std::ranges::size(outer) - 1) * std::ranges::size(contraction);
            for (auto &&it : outer) {
                size += std::ranges::size(std::forward<decltype(it)>(it));
            }
            return size;
        }

      private:
        Outer outer;
        std::ranges::views::all_t<P> contraction;
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
    JoinWithView(R &&, P &&) -> JoinWithView<std::ranges::views::all_t<R>, std::ranges::views::all_t<P>>;

    namespace views {
        struct JoinWithInvoker {
            /**
             * @brief Creates a JoinWithView by joining a range with a contraction element.
             *
             * This function takes a range and a contraction element, forwarding them to create
             * a JoinWithView. The function ensures that the range and contraction elements are
             * correctly transformed into views before constructing the resultant view.
             *
             * @param range The input range to be joined.
             * @param contraction The element used as the separator between elements of the range.
             * @return A JoinWithView representing the joined range with the specified contraction.
             */
            template <std::ranges::input_range R, std::ranges::forward_range P>
                requires std::ranges::viewable_range<R> &&
                         std::ranges::input_range<std::ranges::range_reference_t<R>> &&
                         std::ranges::viewable_range<P> && Concatable<std::ranges::range_reference_t<R>, P>
            constexpr auto operator()(R &&range, P &&contraction) const {
                return JoinWithView(std::ranges::views::all(std::forward<R>(range)),
                                    std::ranges::views::all(std::forward<P>(contraction)));
            }

            /**
             * @brief Joins a range with a single contraction value.
             *
             * This function forwards a range and a contraction value to create a new
             * range where each element in the input range is joined with the provided
             * contraction using `std::ranges::views::single`.
             *
             * @param range The input range to be joined.
             * @param contraction The value to be used for joining elements of the range.
             * @return A new range resulting from joining the input range with the contraction value.
             */
            template <std::ranges::input_range R, typename P>
                requires std::ranges::viewable_range<R> &&
                         std::ranges::input_range<std::ranges::range_reference_t<R>> &&
                         std::convertible_to<P, std::ranges::range_value_t<std::ranges::range_reference_t<R>>>
            constexpr auto operator()(R &&range, P &&contraction) const {
                return (*this)(std::forward<R>(range), std::ranges::views::single(std::forward<P>(contraction)));
            }

            template <std::ranges::input_range R, typename P>
                requires std::ranges::viewable_range<R> &&
                         std::ranges::input_range<std::ranges::range_reference_t<R>> &&
                         std::convertible_to<P, std::ranges::range_value_t<std::ranges::range_reference_t<R>>> &&
                         std::constructible_from<std::basic_string_view<P>, const P *>
            constexpr auto operator()(R &&range, const P *contraction) const {
                return (*this)(std::forward<R>(range), std::basic_string_view<P>(contraction));
            }
        };

        constexpr JoinWithInvoker join_with_invoker;

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
        RETROLIB_EXPORT constexpr auto join_with = extension_method<join_with_invoker>;
    } // namespace views
} // namespace retro::ranges
