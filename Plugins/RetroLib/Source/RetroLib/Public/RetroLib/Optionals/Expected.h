// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#if !RETROLIB_WITH_MODULES
#include <type_traits>
#endif

#include "RetroLib/Concepts/Operators.h"
#include "RetroLib/Optionals/OptionalOperations.h"

#ifndef RETROLIB_EXPORT
#define RETROLIB_EXPORT
#endif

namespace Retro {
    RETROLIB_EXPORT template <typename E>
    class TUnexpected {
    public:
        template <typename T = E>
            requires (!std::same_as<std::remove_cvref_t<T>, TUnexpected>) && (!std::same_as<
                         std::remove_cvref_t<T>, std::in_place_t>) && std::constructible_from<E, T>
        constexpr explicit TUnexpected(T &&Arg) : Error(std::forward<T>(Arg)) {
        }

        template <typename... A>
            requires std::constructible_from<E, A...>
        constexpr explicit TUnexpected(std::in_place_t, A &&... Arg) : Error(std::forward<A>(Arg)...) {
        }

        template <typename U, typename... A>
            requires std::constructible_from<E, std::initializer_list<U> &, A...>
        constexpr explicit TUnexpected(std::in_place_t, std::initializer_list<U> InitializerList, A &&... Arg) : Error(
            InitializerList, std::forward<A>(Arg)...) {
        }

        constexpr E &GetError() & noexcept {
            return Error;
        }

        constexpr const E &GetError() const & noexcept {
            return Error;
        }

        constexpr E &&GetError() && noexcept {
            return std::move(Error);
        }

        constexpr const E &&GetError() const && noexcept {
            return std::move(Error);
        }

        constexpr void Swap(TUnexpected &Other) noexcept(std::is_nothrow_swappable_v<E>) requires std::is_swappable_v<
            E> {
            using std::swap;
            swap(Error, Other.Error);
        }

        constexpr friend void swap(TUnexpected &A, TUnexpected &B) noexcept(std::is_nothrow_swappable_v<E>) requires
            std::is_swappable_v<E> {
            A.Swap(B);
        }

        template <typename T>
        constexpr friend bool operator==(const TUnexpected &LHS,
                                         const TUnexpected<T> &RHS) noexcept(noexcept(LHS.GetError() == RHS.GetError()))
            requires EqualityComparable<E, T> {
            return LHS.GetError() == RHS.GetError();
        }

    private:
        E Error;
    };

    RETROLIB_EXPORT template <typename E>
    TUnexpected(E) -> TUnexpected<E>;

    RETROLIB_EXPORT template <typename E>
    class TBadExpectedAccess;

    RETROLIB_EXPORT template <>
    class TBadExpectedAccess<void> : public std::exception {
    protected:
        TBadExpectedAccess() noexcept = default;
        TBadExpectedAccess(const TBadExpectedAccess &) = default;
        TBadExpectedAccess(TBadExpectedAccess &&) noexcept = default;
        ~TBadExpectedAccess() override = default;
        TBadExpectedAccess &operator=(const TBadExpectedAccess &) = default;
        TBadExpectedAccess &operator=(TBadExpectedAccess &&) noexcept = default;

    public:
        const char *what() const noexcept override {
            return "Bad Expected";
        }
    };

    template <typename E>
    class TBadExpectedAccess : public TBadExpectedAccess<void> {
    public:
        constexpr explicit TBadExpectedAccess(E Arg) : Value(std::move(Arg)) {
        }

        constexpr E &Error() & noexcept {
            return Value;
        }

        constexpr const E &Error() const & noexcept {
            return Value;
        }

        constexpr E &&Error() && noexcept {
            return std::move(Value);
        }

        constexpr const E &&Error() const && noexcept {
            return std::move(Value);
        }

    private:
        E Value;
    };

    RETROLIB_EXPORT struct FUnexpectType {
        explicit FUnexpectType() = default;
    };

    RETROLIB_EXPORT inline constexpr FUnexpectType Unexpect;

    template <typename T>
    concept NonVoidDestructible = std::same_as<T, void> || std::destructible<T>;

    RETROLIB_EXPORT template <NonVoidDestructible T, std::destructible E>
    class TExpected;

    template <typename T, typename E, typename U, typename G, typename UF, typename GF>
    concept ExpectedConstructibleFromOther = std::constructible_from<T, UF> &&
                                             std::constructible_from<E, GF> &&
                                             (!std::constructible_from<T, TExpected<U, G> &>) &&
                                             (!std::constructible_from<T, TExpected<U, G>>) &&
                                             (!std::constructible_from<T, const TExpected<U, G> &>) &&
                                             (!std::constructible_from<T, const TExpected<U, G>>) &&
                                             (!std::convertible_to<TExpected<U, G> &, T>) &&
                                             (!std::convertible_to<TExpected<U, G> &&, T>) &&
                                             (!std::convertible_to<const TExpected<U, G> &, T>) &&
                                             (!std::convertible_to<const TExpected<U, G> &&, T>) &&
                                             (!std::constructible_from<TUnexpected<E>, TExpected<U, G> &>) &&
                                             (!std::constructible_from<TUnexpected<E>, TExpected<U, G>>) &&
                                             (!std::constructible_from<TUnexpected<E>, const TExpected<U, G> &>) &&
                                             (!std::constructible_from<TUnexpected<E>, const TExpected<U, G>>);

    template <typename T>
    concept IsUnexpected = std::same_as<std::remove_cvref_t<T>, TUnexpected<typename T::ValueType>>;

    template <typename T>
    concept IsExpected = std::same_as<std::remove_cvref_t<T>, TExpected<typename T::ValueType, typename T::ErrorType>>;

    template <typename T, typename U, typename... A>
    constexpr void ReinitExpected(T &NewVal, U &OldVal, A &&... Args) {
        if constexpr (std::is_nothrow_constructible_v<T, A...>) {
            std::destroy_at(std::addressof(OldVal));
            std::construct_at(std::addressof(NewVal), std::forward<A>(Args)...);
        } else if constexpr (std::is_nothrow_move_constructible_v<T>) {
            T Temp(std::forward<A>(Args)...);
            std::destroy_at(std::addressof(OldVal));
            std::construct_at(std::addressof(NewVal), std::move(Temp));
        } else {
            U Temp(std::move(OldVal));
            std::destroy_at(std::addressof(OldVal));
            try {
                std::construct_at(std::addressof(NewVal), std::forward<A>(Args)...);
            } catch (...) {
                std::construct_at(std::addressof(NewVal), std::forward<A>(Args)...);
                throw;
            }
        }
    }

    template <NonVoidDestructible T, std::destructible E>
    class TExpected {
        template <NonVoidDestructible U, std::destructible G>
        friend class TExpected;

    public:
        using ValueType = T;
        using ErrorType = E;
        using UnexpectedType = TUnexpected<E>;

        template <typename U>
        using Rebind = TExpected<U, ErrorType>;

        constexpr TExpected() requires std::default_initializable<T> : Value() {
        }

        constexpr TExpected(const TExpected &) requires
            std::copy_constructible<T> && std::copy_constructible<E> && std::is_trivially_copy_constructible_v<T> &&
            std::is_trivially_copy_constructible_v<E> = default;

        constexpr TExpected(const TExpected &Other) requires
            std::copy_constructible<T> && std::copy_constructible<E> : IsValid(Other.IsValid) {
            if (Other.IsValid) {
                std::construct_at(std::addressof(Value), Other.Value);
            } else {
                std::construct_at(std::addressof(Error), Other.Error);
            }
        }

        constexpr TExpected(TExpected &&Other) noexcept requires
            std::move_constructible<T> && std::move_constructible<E> && std::is_trivially_move_constructible_v<T> &&
            std::is_trivially_move_constructible_v<E> = default;

        constexpr TExpected(
            TExpected &&Other) noexcept(std::is_nothrow_move_constructible_v<T> && std::is_nothrow_move_constructible_v<
                                            E>) requires
            std::move_constructible<T> && std::move_constructible<E> : IsValid(Other.IsValid) {
            if (Other.IsValid) {
                std::construct_at(std::addressof(Value), std::move(Other.Value));
            } else {
                std::construct_at(std::addressof(Error), std::move(Other.Error));
            }
        }

        template <typename U, typename G>
            requires ExpectedConstructibleFromOther<T, E, U, G, const U &, const G &>
        constexpr explicit(!std::convertible_to<const U &, T> || std::convertible_to<const G &, E>) TExpected(
            const TExpected<G, U> &Other) {
            if (Other.IsSet()) {
                std::construct_at(std::addressof(Value), Other.Value);
            } else {
                std::construct_at(std::addressof(Error), Other.Error);
            }
        }

        template <typename U, typename G>
            requires ExpectedConstructibleFromOther<T, E, U, G, U, G>
        constexpr explicit(!std::convertible_to<const U &, T> || std::convertible_to<const G &, E>) TExpected(
            TExpected<G, U> &&Other) {
            if (Other.IsSet()) {
                std::construct_at(std::addressof(Value), std::move(Other.Value));
            } else {
                std::construct_at(std::addressof(Error), std::move(Other.Error));
            }
        }

        template <typename U = T>
            requires (!std::same_as<std::remove_cvref_t<U>, std::in_place_t>) &&
                     (!std::same_as<std::remove_cvref_t<U>, TExpected>) &&
                     (!IsUnexpected<U>) &&
                     std::constructible_from<T, U>
        constexpr explicit(!std::convertible_to<U, T>) TExpected(U &&Val) : Value(std::forward<U>(Val)) {
        }

        template <typename G>
            requires std::constructible_from<E, const G &>
        constexpr explicit(!std::convertible_to<const G &, E>)
        TExpected(const TUnexpected<G> &Err) : IsValid(false), Error(Err.Error()) {
        }

        template <typename G>
            requires std::constructible_from<E, G>
        constexpr explicit(!std::convertible_to<G, E>) TExpected(TUnexpected<G> &&Err) : IsValid(false),
            Error(std::move(Err.GetError())) {
        }

        template <typename... A>
            requires std::constructible_from<T, A...>
        constexpr explicit TExpected(std::in_place_t, A &&... Args) : Value(std::forward<A>(Args)...) {
        }

        template <typename U, typename... A>
            requires std::constructible_from<T, std::initializer_list<U> &, A...>
        constexpr explicit TExpected(std::in_place_t, std::initializer_list<U> Initializers, A &&... Args) : Value(
            Initializers, std::forward<A>(Args)...) {
        }

        template <typename... A>
            requires std::constructible_from<E, A...>
        constexpr explicit TExpected(FUnexpectType, A &&... Args) : IsValid(false), Error(std::forward<A>(Args)...) {
        }

        template <typename U, typename... A>
            requires std::constructible_from<E, std::initializer_list<U> &, A...>
        constexpr explicit
        TExpected(FUnexpectType, std::initializer_list<U> Initializers, A &&... Args) : IsValid(false),
            Error(Initializers, std::forward<A>(Args)...) {
        }

        constexpr ~TExpected() requires std::is_trivially_destructible_v<T> && std::is_trivially_destructible_v<E>
        = default;

        constexpr ~TExpected() requires (!std::is_trivially_destructible_v<T>) || (!std::is_trivially_destructible_v<
                                            E>) {
            if constexpr (std::is_trivially_destructible_v<T>) {
                if (!IsValid) {
                    std::destroy_at(std::addressof(Error));
                }
            } else if constexpr (std::is_trivially_destructible_v<E>) {
                if (IsValid) {
                    std::destroy_at(std::addressof(Value));
                }
            } else {
                if (IsValid) {
                    std::destroy_at(std::addressof(Value));
                } else {
                    std::destroy_at(std::addressof(Error));
                }
            }
        }

        constexpr TExpected &operator=(const TExpected &) requires
            std::is_trivially_copy_assignable_v<T> && std::is_trivially_copy_assignable_v<E> = default;

        constexpr TExpected &operator=(const TExpected &Other) requires
            std::is_copy_constructible_v<T> && std::is_copy_assignable_v<T> && std::is_copy_constructible_v<E> &&
            std::is_copy_assignable_v<E> && (std::is_nothrow_move_constructible_v<E> ||
                                             std::is_nothrow_move_constructible_v<T>) && (
                !std::is_trivially_copy_assignable_v<T> || !std::is_trivially_copy_assignable_v<E>) {
            IsValid = Other.IsValid;
            if (IsValid && Other.IsValid) {
                Value = Other.Value;
            } else if (IsValid) {
                ReinitExpected(Error, Value, Other.Error);
            } else if (Other.IsValid) {
                ReinitExpected(Value, Error, Other.Value);
            } else {
                Error = Other.Error;
            }

            return *this;
        }

        constexpr TExpected &operator=(TExpected &&) noexcept requires
            std::is_trivially_move_assignable_v<T> && std::is_trivially_move_assignable_v<E> = default;

        constexpr TExpected &operator
        =(TExpected &&Other) noexcept(std::is_nothrow_move_assignable_v<T> && std::is_nothrow_move_constructible_v<T> &&
                                      std::is_nothrow_move_assignable_v<E> && std::is_nothrow_move_constructible_v<E>)
            requires std::is_move_constructible_v<T> && std::is_move_assignable_v<T> && std::is_move_constructible_v<E>
                     && std::is_move_assignable_v<E> && (
                         std::is_nothrow_move_constructible_v<T> || std::is_nothrow_move_constructible_v<E>) && (
                         !std::is_trivially_move_assignable_v<T> || !std::is_trivially_move_assignable_v<E>) {
            IsValid = Other.IsValid;
            if (IsValid && Other.IsValid) {
                Value = std::move(Other.Value);
            } else if (IsValid) {
                ReinitExpected(Error, Value, std::move(Other.Error));
            } else if (Other.IsValid) {
                ReinitExpected(Value, Error, std::move(Other.Value));
            } else {
                Error = std::move(Other.Error);
            }

            return *this;
        }

        template <typename U = T>
            requires (!std::same_as<TExpected, std::remove_cvref_t<U>>) &&
                     (!IsUnexpected<std::remove_cvref_t<U>>) &&
                     std::constructible_from<T, U> &&
                     std::assignable_from<T &, U> &&
                     (std::is_nothrow_constructible_v<T, U> || std::is_nothrow_move_constructible_v<T> ||
                      std::is_nothrow_move_constructible_v<E>)
        constexpr TExpected &operator=(U &&Other) {
            if (IsValid) {
                Value = std::forward<U>(Other);
            } else {
                ReinitExpected(Value, Error, std::forward<U>(Other));
                IsValid = true;
            }
            return *this;
        }

        template <typename G>
            requires std::constructible_from<E, const G &> &&
                     std::assignable_from<E &, const G &> &&
                     (std::is_nothrow_constructible_v<E, const G &> ||
                      std::is_nothrow_move_constructible_v<T> ||
                      std::is_nothrow_move_constructible_v<E>)
        constexpr TExpected &operator=(const TUnexpected<G> &Other) {
            if (IsValid) {
                ReinitExpected(Error, Value, Other.Error);
                IsValid = false;
            } else {
                Error = Other.Error;
            }

            return *this;
        }

        template <typename G>
            requires std::constructible_from<E, G> &&
                     std::assignable_from<E &, G> &&
                     (std::is_nothrow_constructible_v<E, G> ||
                      std::is_nothrow_move_constructible_v<T> ||
                      std::is_nothrow_move_constructible_v<E>)
        constexpr TExpected &operator=(TUnexpected<G> &&Other) {
            if (IsValid) {
                ReinitExpected(Error, Value, std::move(Other.GetError()));
                IsValid = false;
            } else {
                Error = std::move(Other.GetError());
            }

            return *this;
        }

        template <typename... A>
            requires std::is_nothrow_constructible_v<T, A...>
        constexpr T &Emplace(A &&... Args) noexcept {
            if (IsValid) {
                std::destroy_at(std::addressof(Value));
            } else {
                std::destroy_at(std::addressof(Error));
                IsValid = true;
            }

            return *std::construct_at(std::addressof(Value), std::forward<A>(Args)...);
        }

        template <typename U, typename... A>
            requires std::is_nothrow_constructible_v<T, std::initializer_list<U> &, A...>
        constexpr T &Emplace(std::initializer_list<U> Initializers, A &&... Args) noexcept {
            if (IsValid) {
                std::destroy_at(std::addressof(Value));
            } else {
                std::destroy_at(std::addressof(Error));
                IsValid = true;
            }

            return *std::construct_at(std::addressof(Value), Initializers, std::forward<A>(Args)...);
        }

        constexpr void Swap(
            TExpected &Other) noexcept(std::is_nothrow_constructible_v<T> && std::is_nothrow_swappable_v<T> &&
                                       std::is_nothrow_move_constructible_v<E> && std::is_nothrow_swappable_v<E>)
            requires std::is_swappable_v<T> && std::is_swappable_v<E> && std::is_move_constructible_v<T> &&
                     std::is_move_constructible_v<E> && (
                         std::is_nothrow_constructible_v<T> || std::is_nothrow_constructible_v<E>) {
            if (Other.IsValid) {
                if (IsValid) {
                    using std::swap;
                    swap(Value, Other.Value);
                } else {
                    Other.Swap(*this);
                }
            } else {
                if (IsValid) {
                    if constexpr (std::is_nothrow_move_constructible_v<E>) {
                        E Temp = std::move(Other.Error);
                        std::destroy_at(std::addressof(Other.Error));
                        try {
                            std::construct_at(std::addressof(Other.Value), std::move(Value));
                            std::destroy_at(std::addressof(Value));
                            std::construct_at(std::addressof(Error), std::move(Temp));
                        } catch (...) {
                            std::construct_at(std::addressof(Error), std::move(Temp));
                            throw;
                        }
                    } else {
                        T Temp = std::move(Value);
                        std::destroy_at(std::addressof(Value));
                        try {
                            std::construct_at(std::addressof(Error), std::move(Other.Error));
                            std::destroy_at(std::addressof(Other.Error));
                            std::construct_at(std::addressof(Other.Value), std::move(Temp));
                        } catch (...) {
                            std::construct_at(std::addressof(Value), std::move(Temp));
                            throw;
                        }
                    }
                    IsValid = false;
                    Other.IsValid = true;
                } else {
                    using std::swap;
                    swap(Error, Other.Error);
                }
            }
        }

        constexpr T *operator->() noexcept {
            RETROLIB_ASSERT(IsValid);
            return std::addressof(Value);
        }

        constexpr const T *operator->() const noexcept {
            RETROLIB_ASSERT(IsValid);
            return std::addressof(Value);
        }

        constexpr T &operator*() & noexcept {
            RETROLIB_ASSERT(IsValid);
            return Value;
        }

        constexpr const T &operator*() const & noexcept {
            RETROLIB_ASSERT(IsValid);
            return Value;
        }

        constexpr T &&operator*() && noexcept {
            RETROLIB_ASSERT(IsValid);
            return std::move(Value);
        }

        constexpr const T &&operator*() const && noexcept {
            RETROLIB_ASSERT(IsValid);
            return std::move(Value);
        }

        [[nodiscard]] constexpr bool IsSet() const noexcept {
            return IsValid;
        }

        constexpr T &GetValue() & {
            if (IsValid) {
                return Value;
            }

            throw TBadExpectedAccess(Error);
        }

        constexpr const T &GetValue() const & {
            if (IsValid) {
                return Value;
            }

            throw TBadExpectedAccess(Error);
        }

        constexpr T &&GetValue() && {
            if (IsValid) {
                return std::move(Value);
            }

            throw TBadExpectedAccess(std::move(Error));
        }

        constexpr const T &&GetValue() const && {
            if (IsValid) {
                return std::move(Value);
            }

            throw TBadExpectedAccess(std::move(Error));
        }

        constexpr E &GetError() & noexcept {
            RETROLIB_ASSERT(!IsValid);
            return Error;
        }

        constexpr const E &GetError() const & noexcept {
            RETROLIB_ASSERT(!IsValid);
            return Error;
        }

        constexpr E &&GetError() && noexcept {
            RETROLIB_ASSERT(!IsValid);
            return std::move(Error);
        }

        constexpr const E &&GetError() const && noexcept {
            RETROLIB_ASSERT(!IsValid);
            return std::move(Error);
        }

        template <typename U, typename G>
            requires (!std::is_void_v<U>) && EqualityComparable<T, U> && EqualityComparable<E, G>
        friend constexpr bool operator==(const TExpected &LHS, const TExpected<U, G> &RHS) noexcept {
            if (LHS.IsSet() != RHS.IsSet()) {
                return false;
            }

            return LHS.IsSet() ? *LHS == *RHS : LHS.GetError() == RHS.GetError();
        }

        template <typename U>
            requires (!IsExpected<U>) && EqualityComparable<T, U>
        friend constexpr bool operator==(const TExpected &LHS, const U &RHS) {
            return LHS.IsSet() && *LHS == RHS;
        }

        template <typename G>
            requires EqualityComparable<E, G>
        friend constexpr bool operator==(const TExpected &LHS, const TUnexpected<G> &RHS) {
            return !LHS.IsSet() && LHS.Error() == RHS;
        }

        friend constexpr void swap(TExpected &LHS, TExpected &RHS) noexcept(noexcept(LHS.Swap(RHS))) {
            LHS.Swap(RHS);
        }

    private:
        bool IsValid = true;

        union {
            T Value;
            E Error;
        };
    };

    template <std::destructible E>
    class TExpected<void, E> {
        template <NonVoidDestructible U, std::destructible G>
        friend class TExpected;

    public:
        using ValueType = void;
        using ErrorType = E;
        using UnexpectedType = TUnexpected<E>;

        template <typename U>
        using Rebind = TExpected<U, ErrorType>;

        constexpr TExpected() noexcept {
        }

        constexpr TExpected(const TExpected &) requires
            std::is_copy_constructible_v<E> && std::is_trivially_copy_constructible_v<E> = default;

        constexpr TExpected(const TExpected &Other) requires
            std::is_copy_constructible_v<E> && (!std::is_trivially_copy_constructible_v<E>) : IsValid(Other.IsValid) {
            if (!Other.IsValid) {
                std::construct_at(std::addressof(Other.Error), Error);
            }
        }

        constexpr TExpected(TExpected &&) requires
            std::is_move_constructible_v<E> && std::is_trivially_move_constructible_v<E> = default;

        constexpr TExpected(TExpected &&Other) requires
            std::is_move_constructible_v<E> && (!std::is_trivially_move_constructible_v<E>) : IsValid(Other.IsValid) {
            if (!Other.IsValid) {
                std::construct_at(std::addressof(Other.Error), std::move(Error));
            }
        }

        template <typename U, typename G>
            requires std::is_void_v<U>
                     && std::is_constructible_v<E, const G &>
                     && (!std::is_constructible_v<TUnexpected<E>, TExpected<U, G> &>)
                     && (!std::is_constructible_v<TUnexpected<E>, TExpected<U, G>>)
                     && (!std::is_constructible_v<TUnexpected<E>, const TExpected<U, G> &>)
                     && (!std::is_constructible_v<TUnexpected<E>, const TExpected<U, G> &>)
        constexpr explicit(!std::is_convertible_v<const G &, E>) TExpected(const TExpected<U, G> &Other) {
            if (!Other.IsValid) {
                std::construct_at(std::addressof(Error), Other.Error);
            }
        }

        template <class U, class G>
            requires std::is_void_v<U>
                     && std::is_constructible_v<E, G>
                     && (!std::is_constructible_v<TUnexpected<E>, TExpected<U, G> &>)
                     && (!std::is_constructible_v<TUnexpected<E>, TExpected<U, G>>)
                     && (!std::is_constructible_v<TUnexpected<E>, const TExpected<U, G> &>)
                     && (!std::is_constructible_v<TUnexpected<E>, const TExpected<U, G> &>)
        constexpr explicit(!std::is_convertible_v<const G &, E>) TExpected(TExpected<U, G> &&Other) : IsValid(
            Other.IsValid) {
            if (!Other.IsValid) {
                std::construct_at(std::addressof(Error), std::move(Other.Error));
            }
        }

        template <typename G>
            requires std::constructible_from<E, const G &>
        constexpr explicit(!std::convertible_to<const G &, E>) TExpected(const TUnexpected<G> &Other) : IsValid(
            Other.IsValid) {
            if (!Other.IsValid) {
                std::construct_at(std::addressof(Error), Other.GetError());
            }
        }

        template <typename G>
            requires std::constructible_from<E, G>
        constexpr explicit(!std::convertible_to<G, E>) TExpected(TUnexpected<G> &&Other) : IsValid(Other.IsValid) {
            if (!Other.IsValid) {
                std::construct_at(std::addressof(Error), std::move(Other.GetError()));
            }
        }

        constexpr explicit TExpected(std::in_place_t) noexcept {
        }

        template <typename... A>
            requires std::is_constructible_v<E, A...>
        constexpr explicit TExpected(FUnexpectType, A &&... Args) : IsValid(false), Error(std::forward<A>(Args)...) {
        }

        template <typename U, typename... A>
            requires std::is_constructible_v<E, std::initializer_list<U> &, A...>
        constexpr explicit
        TExpected(FUnexpectType, std::initializer_list<U> Initializers, A &&... Args) : IsValid(false),
            Error(Initializers, std::forward<A>(Args)...) {
        }

        constexpr ~TExpected() requires std::is_trivially_destructible_v<E> = default;

        constexpr ~TExpected() requires (!std::is_trivially_destructible_v<E>) {
            if (!IsValid) {
                std::destroy_at(std::addressof(Error));
            }
        }

        constexpr TExpected &operator=(const TExpected &) requires std::is_trivially_copy_assignable_v<E> = default;

        constexpr TExpected &operator=(const TExpected &Other) requires
            std::is_copy_assignable_v<E> && std::is_copy_constructible_v<E> && (!std::is_trivially_copy_assignable_v<
                E>) {
            if (IsValid && Other.IsValid) {
                // Do nothing
            } else if (IsValid) {
                std::construct_at(std::addressof(Error), Other.Error);
                IsValid = false;
            } else if (Other.IsValid) {
                std::destroy_at(Error);
                IsValid = true;
            } else {
                Error = Other.Error;
            }

            return *this;
        }

        constexpr TExpected &operator=(TExpected &&) requires std::is_trivially_move_assignable_v<E> = default;

        constexpr TExpected &operator=(const TExpected &Other) requires
            std::is_copy_assignable_v<E> && std::is_move_assignable_v<E> && (!std::is_trivially_move_assignable_v<E>) {
            if (IsValid && Other.IsValid) {
                // Do nothing
            } else if (IsValid) {
                std::construct_at(std::addressof(Error), std::move(Other.Error));
                IsValid = false;
            } else if (Other.IsValid) {
                std::destroy_at(Error);
                IsValid = true;
            } else {
                Error = std::move(Other.Error);
            }

            return *this;
        }

        template <typename G>
            requires std::constructible_from<E, const G &> && std::assignable_from<E &, const G &>
        constexpr TExpected &operator=(const TUnexpected<G> &Other) {
            if (IsValid) {
                std::construct_at(std::addressof(Error), Other.GetError());
                IsValid = false;
            } else {
                Error = Other.Error;
            }

            return *this;
        }

        template <typename G>
            requires std::constructible_from<E, G> && std::assignable_from<E &, G>
        constexpr TExpected &operator=(TUnexpected<G> &&Other) {
            if (IsValid) {
                std::construct_at(std::addressof(Error), std::move(Other.GetError()));
                IsValid = false;
            } else {
                Error = std::move(Other.GetError());
            }

            return *this;
        }

        constexpr void Emplace() noexcept {
            if (!IsValid) {
                std::destroy_at(std::addressof(Error));
            }
        }

        constexpr void Swap(
            TExpected &Other) noexcept(std::is_nothrow_move_constructible_v<E> && std::is_nothrow_swappable_v<E>)
            requires std::is_swappable_v<E> && std::is_move_constructible_v<E> {
            if (Other.IsValid) {
                if (!IsValid) {
                    Other.Swap(*this);
                }
            } else {
                if (IsValid) {
                    std::construct_at(std::addressof(Error), std::move(Other.Error));
                    std::destroy_at(std::addressof(Other.Error));
                    IsValid = false;
                    Other.IsValid = true;
                } else {
                    using std::swap;
                    swap(Error, Other.Error);
                }
            }
        }

        [[nodiscard]] constexpr bool IsSet() const noexcept {
            return IsValid;
        }

        constexpr void operator*() const noexcept {
            // I don't get why this is here, but it is in the specification.
        }

        constexpr void GetValue() const & {
            if (!IsValid) {
                throw TBadExpectedAccess(Error);
            }
        }

        constexpr void GetValue() && {
            if (!IsValid) {
                throw TBadExpectedAccess(std::move(Error));
            }
        }

        constexpr E &GetError() & noexcept {
            RETROLIB_ASSERT(!IsValid);
            return Error;
        }

        constexpr const E &GetError() const & noexcept {
            RETROLIB_ASSERT(!IsValid);
            return Error;
        }

        constexpr E &&GetError() && noexcept {
            RETROLIB_ASSERT(!IsValid);
            return std::move(Error);
        }

        constexpr const E &&GetError() const && noexcept {
            RETROLIB_ASSERT(!IsValid);
            return std::move(Error);
        }

        template <typename U, typename G>
            requires std::is_void_v<U> && EqualityComparable<E, G>
        friend constexpr bool operator==(const TExpected &LHS, const TExpected<U, G> &RHS) {
            if (LHS.IsSet() != RHS.IsSet()) {
                return false;
            }

            return LHS.IsSet() || LHS.GetError() == RHS.GetError();
        }

        template <typename G>
            requires EqualityComparable<E, G>
        friend constexpr bool operator==(const TExpected &LHS, const G &RHS) {
            return !LHS.IsSet() && LHS.Error() == RHS.Error();
        }

        friend constexpr void swap(TExpected &LHS, TExpected &RHS) noexcept {
            LHS.Swap(RHS);
        }

    private:
        bool IsValid = true;

        union {
            E Error;
        };
    };

    template <NonVoidDestructible T>
    struct Optionals::TErrorPassthrough<T> {
        template <typename E>
            requires std::constructible_from<T, TUnexpected<std::remove_cvref_t<E>>>
        constexpr T operator()(E&& Error) const {
            return T(TUnexpected(std::forward<E>(Error)));
        }
    };
}