// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "RetroLib/Functional/ExtensionMethods.h"
#include "RetroLib/Ranges/RangeBasics.h"
#include "RetroLib/Utils/NonPropagatingCache.h"

#if !RETROLIB_WITH_MODULES
#include <memory>
#endif

#ifndef RETROLIB_EXPORT
#define RETROLIB_EXPORT
#endif

namespace Retro::Ranges {

    /**
     * @class TSharedView
     * @brief A custom view implementation that encapsulates a shared ownership of a range.
     *
     * TSharedView provides a mechanism to wrap a range and share it across multiple instances
     * while maintaining a shared ownership. It is derived from `std::ranges::view_interface`
     * and offers functionality that adheres to the C++ standard range concepts.
     *
     * @tparam R Type of the range being wrapped.
     */
    RETROLIB_EXPORT template <std::ranges::input_range R>
    class TSharedView : public std::ranges::view_interface<TSharedView<R>> {

        template <bool Const>
        class TIterator {
            using BaseType = std::conditional_t<Const, const R, R>;

          public:
            using iterator_category = std::input_iterator_tag;
            using iterator_concept = std::conditional_t<
                std::ranges::random_access_range<BaseType>, std::random_access_iterator_tag,
                std::conditional_t<std::ranges::bidirectional_range<BaseType>, std::bidirectional_iterator_tag,
                                   std::conditional_t<std::ranges::forward_range<BaseType>, std::forward_iterator_tag,
                                                      std::input_iterator_tag>>>;
            using difference_type = std::ranges::range_difference_t<BaseType>;
            using value_type = std::ranges::range_value_t<BaseType>;

          private:
            using ReferenceType = std::ranges::range_reference_t<BaseType>;

          public:
            constexpr TIterator()
                requires std::default_initializable<std::ranges::iterator_t<BaseType>>
            = default;

            constexpr explicit(false) TIterator(TIterator<!Const> Other)
                requires Const && std::convertible_to<std::ranges::iterator_t<R>, std::ranges::iterator_t<BaseType>>
                : Current(std::move(Other.current)) {
            }

          private:
            constexpr explicit TIterator(std::ranges::iterator_t<BaseType> Current)
                : Current(std::move(Current)) {
            }

          public:
            constexpr const std::ranges::iterator_t<BaseType> &base() const & noexcept {
                return Current;
            }

            constexpr std::ranges::iterator_t<BaseType> base() && {
                return std::move(Current);
            }

            constexpr decltype(auto) operator*() const
                noexcept(noexcept(*Current) &&
                         std::is_nothrow_copy_constructible_v<std::ranges::range_reference_t<BaseType>>) {
                return *Current;
            }

            constexpr auto operator[](difference_type N) const
                requires std::ranges::random_access_range<BaseType>
            {
                return Current[N];
            }

            constexpr TIterator &operator++() {
                ++Current;
                return *this;
            }

            constexpr void operator++(int) {
                ++Current;
            }

            constexpr TIterator operator++(int)
                requires std::ranges::forward_range<BaseType>
            {
                TIterator Tmp = *this;
                ++*this;
                return Tmp;
            }

            constexpr TIterator &operator--()
                requires std::ranges::bidirectional_range<BaseType>
            {
                --Current;
                return *this;
            }

            constexpr TIterator operator--(int)
                requires std::ranges::bidirectional_range<BaseType>
            {
                TIterator Tmp = *this;
                --*this;
                return Tmp;
            }

            constexpr TIterator operator+=(difference_type N)
                requires std::ranges::random_access_range<BaseType>
            {
                Current += N;
                return *this;
            }

            constexpr TIterator operator-=(difference_type N)
                requires std::ranges::random_access_range<BaseType>
            {
                Current -= N;
                return *this;
            }

            friend constexpr bool operator==(const TIterator &Lhs, const TIterator &Rhs) noexcept requires std::ranges::forward_range<BaseType> {
                return Lhs.Current == Rhs.Current;
            }

            friend constexpr std::strong_ordering operator<=>(const TIterator &Lhs, const TIterator &Rhs) noexcept requires std::ranges::random_access_range<BaseType> {
                return Lhs.Current <=> Rhs.Current;
            }

            friend constexpr TIterator operator+(const TIterator &Self, difference_type N)
                requires std::ranges::random_access_range<BaseType>
            {
                auto Temp = Self;
                Temp += N;
                return Temp;
            }

            friend constexpr TIterator operator+(difference_type N, const TIterator &Self)
                requires std::ranges::random_access_range<BaseType>
            {
                return Self + N;
            }

            friend constexpr TIterator operator-(const TIterator &Self, difference_type N)
                requires std::ranges::random_access_range<BaseType>
            {
                auto Temp = Self;
                Temp -= N;
                return Temp;
            }

            friend constexpr difference_type operator-(const TIterator &Lhs, const TIterator &Rhs)
                requires std::ranges::random_access_range<BaseType>
            {
                return Lhs.Current - Rhs.Current;
            }

          private:
            friend class TIterator<!Const>;
            friend class TSharedView;

            std::ranges::iterator_t<BaseType> Current;
        };

        template <bool Const>
        class TSentinel {
            using BaseType = std::conditional_t<Const, const R, R>;

          public:
            constexpr TSentinel() = default;

            constexpr explicit(false) TSentinel(TIterator<!Const> Other)
                requires Const && std::convertible_to<std::ranges::sentinel_t<R>, std::ranges::sentinel_t<BaseType>>
                : End(std::move(Other.end)) {
            }

          private:
            constexpr explicit TSentinel(std::ranges::sentinel_t<BaseType> End) : End(std::move(End)) {
            }

          public:
            constexpr std::ranges::sentinel_t<BaseType> base() const noexcept {
                return End;
            }

            friend constexpr bool operator==(const TIterator<Const> &Lhs, const TSentinel &Rhs) noexcept {
                return Lhs.base() == Rhs.End;
            }

            template <bool OtherConst>
                requires std::sized_sentinel_for<std::ranges::sentinel_t<BaseType>,
                                                 std::ranges::iterator_t<TMaybeConst<OtherConst, R>>>
            friend constexpr auto operator-(const TIterator<OtherConst> &Lhs, const TSentinel &Rhs) {
                return Lhs.base() - Rhs.End;
            }

            template <bool OtherConst>
                requires std::sized_sentinel_for<std::ranges::sentinel_t<BaseType>,
                                                 std::ranges::iterator_t<TMaybeConst<OtherConst, R>>>
            friend constexpr auto operator-(const TSentinel &LHS, const TIterator<OtherConst> &RHS) {
                return LHS.End - RHS.base();
            }

          private:
            friend class TSharedView;
            
            std::ranges::sentinel_t<BaseType> End;
        };
    
    public:
        /**
         * @brief Constructs a `TSharedView` instance from a given range, ensuring shared ownership.
         *
         * This constructor creates a shared instance of the provided range using
         * `std::make_shared` and initializes the base with the shared instance.
         *
         * @tparam R The type of the range being wrapped. Must be `std::copy_constructible`.
         * @param Range The range to be shared and wrapped by the `TSharedView`.
         */
        explicit constexpr TSharedView(const R& Range) requires std::copy_constructible<R> : Range(std::make_shared<R>(Range)) {}

        /**
         * @brief Constructs a `TSharedView` instance by wrapping a range with shared ownership.
         *
         * This constructor initializes the `TSharedView` with a shared instance of the provided
         * range, ensuring the range is safely shared among multiple instances.
         *
         * @tparam R The type of the range being wrapped. Must satisfy `std::ranges::input_range`.
         * @param Range The range to be wrapped in a shared instance and managed by `TSharedView`.
         */
        explicit constexpr TSharedView(R&& Range) : Range(std::make_shared<R>(std::move(Range))) {}

        /**
         * @brief Constructs a `TSharedView` instance with shared ownership from a specified type and arguments.
         *
         * This constructor creates a shared instance of the provided type `T` using the forwarded arguments
         * and initializes the `TSharedView` with it. `T` must derive from the range type `R` and be constructible
         * with the provided arguments.
         *
         * @tparam T The concrete type of the range to be wrapped. Must be derived from `R`.
         * @tparam A The types of the arguments used to construct an instance of `T`.
         * @param Args The arguments used to construct the instance of type `T`.
         */
        template <typename T, typename... A>
            requires std::derived_from<T, R> && std::constructible_from<T, A...>
        explicit constexpr TSharedView(std::in_place_type_t<T>, A&&... Args) : Range(std::make_shared<T>(std::forward<A>(Args)...)) {}

        /**
         * @brief Provides access to the underlying range managed by the `TSharedView`.
         *
         * This method returns a reference to the range encapsulated within the `TSharedView`,
         * allowing direct access to the wrapped range object.
         *
         * @return R& A reference to the underlying range managed by the `TSharedView`.
         */
        constexpr R& base() {
            return *Range;
        }

        /**
         * @brief Returns a const reference to the underlying range managed by the `TSharedView`.
         *
         * This method provides read-only access to the wrapped range object, ensuring
         * the caller can inspect or use the range while maintaining the shared ownership model.
         *
         * @return const R& A const reference to the underlying range managed by the `TSharedView`.
         */
        constexpr const R& base() const {
            return *Range;
        }

        /**
         * @brief Returns an iterator pointing to the beginning of the container.
         *
         * This method provides access to the starting iterator of the container,
         * enabling traversal of its elements from the first position.
         *
         * @return An iterator to the beginning of the container.
         */
        constexpr auto begin()
            requires(!SimpleView<R>)
        {
            return TIterator<false>(std::ranges::begin(*Range));
        }

        /**
         * @brief Returns a constant iterator pointing to the beginning of the range.
         *
         * This method provides a constant iterator to the start of the range managed by
         * the `TSharedView`, ensuring it adheres to the constraints of ranges with movable references.
         *
         * @return A constant iterator to the beginning of the range.
         * @tparam R The type of the range. Must satisfy `RangeWithMovableReference<const R>`.
         */
        constexpr auto begin() const
            requires RangeWithMovableReference<const R>
        {
            return TIterator<true>(std::ranges::begin(std::as_const(*Range)));
        }

        constexpr auto end()
            requires(!SimpleView<R>)
        {
            if constexpr (std::ranges::forward_range<R> && std::ranges::common_range<R>) {
                return TIterator<false>(std::ranges::end(*Range));
            } else {
                return TSentinel<false>(std::ranges::end(*Range));
            }
        }

        constexpr auto end() const
            requires RangeWithMovableReference<const R>
        {
            if constexpr (std::ranges::forward_range<R>) {
                return TIterator<true>(std::ranges::end(std::as_const(*Range)));
            } else {
                return TSentinel<true>(std::ranges::end(std::as_const(*Range)));
            }
        }

    private:
        std::shared_ptr<R> Range;
    };

    template <std::ranges::input_range R>
    TSharedView(R&&) -> TSharedView<std::decay_t<R>>;

    namespace Views {
        struct FSharedInvoker {
            template <std::ranges::input_range R>
            constexpr auto operator()(R&& Range) const {
                return TSharedView(std::forward<R>(Range));
            }
        };

        RETROLIB_EXPORT constexpr auto Shared = ExtensionMethod<FSharedInvoker{}>();
    }
}