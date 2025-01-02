// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#ifdef __UNREAL__
#include "RetroLib/TypeTraits.h"

#ifndef RETROLIB_EXPORT
#define RETROLIB_EXPORT
#endif

namespace Retro::Delegates {
    /**
     * Concept to check if a delegate is a native (single binding) delegate.
     *
     * @tparam T The type to check if it's a delegate or not
     */
    RETROLIB_EXPORT template <typename T>
    concept NativeUnicastDelegate = requires(T &&Delegate) {
        { TDelegate(std::forward<T>(Delegate)) } -> std::same_as<std::remove_cvref_t<T>>;
    };

    /**
     * Concept to check if a delegate is a dynamic (single binding) delegate.
     *
     * @tparam T The type to check if it's a delegate or not
     */
    RETROLIB_EXPORT template <typename T>
    concept DynamicUnicastDelegate =
        std::is_base_of_v<decltype(TScriptDelegate(std::declval<T>())), std::remove_cvref_t<T>>;

    /**
     * Concept to check if a delegate is a (single binding) delegate.
     *
     * @tparam T The type to check if it's a delegate or not
     */
    RETROLIB_EXPORT template <typename T>
    concept UnicastDelegate = NativeUnicastDelegate<T> || DynamicUnicastDelegate<T>;

    /**
     * Concept to check if a delegate is a native multicast delegate.
     *
     * @tparam T The type to check if it's a delegate or not
     */
    RETROLIB_EXPORT template <typename T>
    concept NativeMulitcastDelegate = requires(T &&Delegate) {
        { TMulticastDelegate(std::forward<T>(Delegate)) } -> std::same_as<std::remove_cvref_t<T>>;
    };

    /**
     * Concept to check if a delegate is a dynamic multicast delegate.
     *
     * @tparam T The type to check if it's a delegate or not
     */
    RETROLIB_EXPORT template <typename T>
    concept DynamicMulitcastDelegate =
        std::is_base_of_v<decltype(TMulticastScriptDelegate(std::declval<T>())), std::remove_cvref_t<T>>;

    /**
     * Concept to check if a delegate is a multicast delegate.
     *
     * @tparam T The type to check if it's a delegate or not
     */
    RETROLIB_EXPORT template <typename T>
    concept MulticastDelegate = NativeMulitcastDelegate<T> || DynamicMulitcastDelegate<T>;

    RETROLIB_EXPORT template <typename T>
    concept NativeDelegate = NativeUnicastDelegate<T> || NativeMulitcastDelegate<T>;

    RETROLIB_EXPORT template <typename T>
    concept DynamicDelegate = DynamicUnicastDelegate<T> || DynamicMulitcastDelegate<T>;

    /**
     * Concept to check if a delegate is a Unreal Engine delegate type
     *
     * @tparam T The type to check if it's a delegate or not
     */
    RETROLIB_EXPORT template <typename T>
    concept UEDelegate = UnicastDelegate<T> || MulticastDelegate<T>;

    template <NativeDelegate, typename, typename...>
    struct TValidFreeBinding : FInvalidType {};

    template <NativeDelegate>
    struct TDelegateBindingTraits;

    template <typename R, typename... A, typename U>
    struct TDelegateBindingTraits<TDelegate<R(A...), U>> {
        template <typename F, typename... B>
        static constexpr bool InvocableFree = std::is_invocable_r_v<R, F, A..., B...>;

        template <typename O, typename F, typename... B>
        static constexpr bool InvocableMember = std::is_invocable_r_v<R, F, O, A..., B...>;
    };

    template <typename... A, typename U>
    struct TDelegateBindingTraits<TDelegate<void(A...), U>> {
        template <typename F, typename... B>
        static constexpr bool InvocableFree = std::is_invocable_v<F, A..., B...>;

        template <typename O, typename F, typename... B>
        static constexpr bool InvocableMember = std::is_invocable_v<F, O, A..., B...>;
    };

    template <typename... A, typename U>
    struct TDelegateBindingTraits<TMulticastDelegate<void(A...), U>> {
        template <typename F, typename... B>
        static constexpr bool InvocableFree = std::is_invocable_v<F, A..., B...>;

        template <typename O, typename F, typename... B>
        static constexpr bool InvocableMember = std::is_invocable_v<F, O, A..., B...>;
    };

    template <typename>
    struct TCanBindSp : std::false_type {};

    template <typename T, ESPMode Mode>
    struct TCanBindSp<TSharedRef<T, Mode>> : std::true_type {};

    template <typename T>
        requires std::is_base_of_v<TSharedFromThis<T>, T>
    struct TCanBindSp<T *> : std::true_type {};

    template <typename T>
        requires std::is_base_of_v<TSharedFromThis<T>, T>
    struct TCanBindSp<const T *> : std::true_type {};

    /**
     * Concept to check if a single-cast delegate is bindable to a target multicast delegate
     */
    RETROLIB_EXPORT template <typename M, typename S>
    concept BindableTo = MulticastDelegate<M> && UnicastDelegate<S> && requires(M &&Delegate, S &&Source) {
        { Delegate.Add(std::forward<S>(Source)) } -> std::same_as<FDelegateHandle>;
    };

    RETROLIB_EXPORT template <typename D, typename F, typename... A>
    concept CanBindStatic = NativeUnicastDelegate<D> && TDelegateBindingTraits<D>::template InvocableFree<F, A...> &&
                            requires(D &Delegate, F &&Functor, A &&...Args) {
                                Delegate.BindStatic(std::forward<F>(Functor), std::forward<A>(Args)...);
                                {
                                    D::CreateStatic(std::forward<F>(Functor), std::forward<A>(Args)...)
                                } -> std::same_as<D>;
                            };

    RETROLIB_EXPORT template <typename D, typename F, typename... A>
    concept CanBindLambda = NativeUnicastDelegate<D> && TDelegateBindingTraits<D>::template InvocableFree<F, A...> &&
                            requires(D &Delegate, F &&Functor, A &&...Args) {
                                Delegate.BindLambda(std::forward<F>(Functor), std::forward<A>(Args)...);
                                {
                                    D::CreateLambda(std::forward<F>(Functor), std::forward<A>(Args)...)
                                } -> std::same_as<D>;
                            };

    RETROLIB_EXPORT template <typename D, typename O, typename F, typename... A>
    concept CanBindRaw =
        NativeUnicastDelegate<D> && TDelegateBindingTraits<D>::template InvocableMember<O, F, A...> &&
        requires(D &Delegate, O &&Object, F &&Functor, A &&...Args) {
            Delegate.BindRaw(std::forward<O>(Object), std::forward<F>(Functor), std::forward<A>(Args)...);
            {
                D::CreateRaw(std::forward<O>(Object), std::forward<F>(Functor), std::forward<A>(Args)...)
            } -> std::same_as<D>;
        };

    RETROLIB_EXPORT template <typename D, typename O, typename F, typename... A>
    concept CanBindSP =
        NativeUnicastDelegate<D> && TCanBindSp<std::decay_t<O>>::value &&
        requires(D &Delegate, O &&Object, F &&Functor, A &&...Args) {
            Delegate.BindSP(std::forward<O>(Object), std::forward<F>(Functor), std::forward<A>(Args)...);
            {
                D::CreateSP(std::forward<O>(Object), std::forward<F>(Functor), std::forward<A>(Args)...)
            } -> std::same_as<D>;
        };

    RETROLIB_EXPORT template <typename D, typename O, typename F, typename... A>
    concept CanBindSPLambda =
        NativeUnicastDelegate<D> && TCanBindSp<std::decay_t<O>>::value &&
        TDelegateBindingTraits<D>::template InvocableFree<F, A...> &&
        requires(D &Delegate, O &&Object, F &&Functor, A &&...Args) {
            Delegate.BindSPLambda(std::forward<O>(Object), std::forward<F>(Functor), std::forward<A>(Args)...);
            {
                D::CreateSPLambda(std::forward<O>(Object), std::forward<F>(Functor), std::forward<A>(Args)...)
            } -> std::same_as<D>;
        };

    RETROLIB_EXPORT template <typename D, typename O, typename F, typename... A>
    concept CanBindUObject =
        NativeUnicastDelegate<D> && std::convertible_to<O, const UObject *> &&
        TDelegateBindingTraits<D>::template InvocableMember<O, F, A...> &&
        requires(D &Delegate, O &&Object, F &&Functor, A &&...Args) {
            Delegate.BindUObject(std::forward<O>(Object), std::forward<F>(Functor), std::forward<A>(Args)...);
            {
                D::CreateUObject(std::forward<O>(Object), std::forward<F>(Functor), std::forward<A>(Args)...)
            } -> std::same_as<D>;
        };

    RETROLIB_EXPORT template <typename D, typename O, typename F, typename... A>
    concept CanBindWeakLambda =
        NativeUnicastDelegate<D> && TDelegateBindingTraits<D>::template InvocableFree<F, A...> &&
        requires(D &Delegate, O &&Object, F &&Functor, A &&...Args) {
            Delegate.BindWeakLambda(std::forward<O>(Object), std::forward<F>(Functor), std::forward<A>(Args)...);
            {
                D::CreateWeakLambda(std::forward<O>(Object), std::forward<F>(Functor), std::forward<A>(Args)...)
            } -> std::same_as<D>;
        };

    RETROLIB_EXPORT template <typename D, typename F, typename... A>
    concept CanBindFree = CanBindStatic<D, F, A...> || CanBindLambda<D, F, A...>;

    RETROLIB_EXPORT template <typename D, typename O, typename F, typename... A>
    concept CanBindMember =
        CanBindSP<D, O, F, A...> || CanBindSPLambda<D, O, F, A...> || CanBindUObject<D, O, F, A...> ||
        CanBindWeakLambda<D, O, F, A...> || CanBindRaw<D, O, F, A...>;

    RETROLIB_EXPORT template <typename D, typename F, typename... A>
    concept CanAddStatic = NativeMulitcastDelegate<D> && TDelegateBindingTraits<D>::template InvocableFree<F, A...> &&
                            requires(D &Delegate, F &&Functor, A &&...Args) {
                                { Delegate.AddStatic(std::forward<F>(Functor), std::forward<A>(Args)...) } -> std::same_as<FDelegateHandle>;
                            };

    RETROLIB_EXPORT template <typename D, typename F, typename... A>
    concept CanAddLambda = NativeMulitcastDelegate<D> && TDelegateBindingTraits<D>::template InvocableFree<F, A...> &&
                            requires(D &Delegate, F &&Functor, A &&...Args) {
        { Delegate.AddLambda(std::forward<F>(Functor), std::forward<A>(Args)...) } -> std::same_as<FDelegateHandle>;
                            };

    RETROLIB_EXPORT template <typename D, typename O, typename F, typename... A>
    concept CanAddRaw =
        NativeMulitcastDelegate<D> && TDelegateBindingTraits<D>::template InvocableMember<O, F, A...> &&
        requires(D &Delegate, O &&Object, F &&Functor, A &&...Args) {
            { Delegate.AddRaw(std::forward<O>(Object), std::forward<F>(Functor), std::forward<A>(Args)...) } -> std::same_as<FDelegateHandle>;
        };

    RETROLIB_EXPORT template <typename D, typename O, typename F, typename... A>
    concept CanAddSP =
        NativeMulitcastDelegate<D> && TCanBindSp<std::decay_t<O>>::value &&
        requires(D &Delegate, O &&Object, F &&Functor, A &&...Args) {
            { Delegate.AddSP(std::forward<O>(Object), std::forward<F>(Functor), std::forward<A>(Args)...) } -> std::same_as<FDelegateHandle>;
        };

    RETROLIB_EXPORT template <typename D, typename O, typename F, typename... A>
    concept CanAddSPLambda =
        NativeMulitcastDelegate<D> && TCanBindSp<std::decay_t<O>>::value &&
        TDelegateBindingTraits<D>::template InvocableFree<F, A...> &&
        requires(D &Delegate, O &&Object, F &&Functor, A &&...Args) {
            { Delegate.AddSPLambda(std::forward<O>(Object), std::forward<F>(Functor), std::forward<A>(Args)...) } -> std::same_as<FDelegateHandle>;
        };

    RETROLIB_EXPORT template <typename D, typename O, typename F, typename... A>
    concept CanAddUObject =
        NativeMulitcastDelegate<D> && std::convertible_to<O, const UObject *> &&
        TDelegateBindingTraits<D>::template InvocableMember<O, F, A...> &&
        requires(D &Delegate, O &&Object, F &&Functor, A &&...Args) {
            {  Delegate.AddUObject(std::forward<O>(Object), std::forward<F>(Functor), std::forward<A>(Args)...) } -> std::same_as<FDelegateHandle>;
        };

    RETROLIB_EXPORT template <typename D, typename O, typename F, typename... A>
    concept CanAddWeakLambda =
        NativeMulitcastDelegate<D> && TDelegateBindingTraits<D>::template InvocableFree<F, A...> &&
        requires(D &Delegate, O &&Object, F &&Functor, A &&...Args) {
            { Delegate.AddWeakLambda(std::forward<O>(Object), std::forward<F>(Functor), std::forward<A>(Args)...) } -> std::same_as<FDelegateHandle>;
        };

    RETROLIB_EXPORT template <typename D, typename F, typename... A>
    concept CanAddFree = CanAddStatic<D, F, A...> || CanAddLambda<D, F, A...>;

    RETROLIB_EXPORT template <typename D, typename O, typename F, typename... A>
    concept CanAddMember =
        CanAddSP<D, O, F, A...> || CanAddSPLambda<D, O, F, A...> || CanAddUObject<D, O, F, A...> ||
        CanAddWeakLambda<D, O, F, A...> || CanAddRaw<D, O, F, A...>;
    
} // namespace retro
#endif