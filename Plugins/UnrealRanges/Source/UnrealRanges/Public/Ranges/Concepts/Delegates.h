// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include <TypeTraits.h>

namespace UE::Ranges {

    /**
     * Concept to check if a delegate is a native (single binding) delegate.
     * @tparam T The type to check if it's a delegate or not
     */
    template <typename T>
    concept NativeUnicastDelegate = requires(T &&Delegate) {
        { TDelegate(std::forward<T>(Delegate)) } -> std::same_as<std::remove_cvref_t<T>>;
    };

    /**
     * Concept to check if a delegate is a dynamic (single binding) delegate.
     * @tparam T The type to check if it's a delegate or not
     */
    template <typename T>
    concept DynamicUnicastDelegate =
        std::is_base_of_v<decltype(TScriptDelegate(std::declval<T>())), std::remove_cvref_t<T>>;

    /**
     * Concept to check if a delegate is a (single binding) delegate.
     * @tparam T The type to check if it's a delegate or not
     */
    template <typename T>
    concept UnicastDelegate = NativeUnicastDelegate<T> || DynamicUnicastDelegate<T>;

    /**
     * Concept to check if a delegate is a native multicast delegate.
     * @tparam T The type to check if it's a delegate or not
     */
    template <typename T>
    concept NativeMulitcastDelegate = requires(T &&Delegate) {
        { TMulticastDelegate(std::forward<T>(Delegate)) } -> std::same_as<std::remove_cvref_t<T>>;
    };

    /**
     * Concept to check if a delegate is a dynamic multicast delegate.
     * @tparam T The type to check if it's a delegate or not
     */
    template <typename T>
    concept DynamicMulitcastDelegate =
        std::is_base_of_v<decltype(TMulticastScriptDelegate(std::declval<T>())), std::remove_cvref_t<T>>;

    /**
     * Concept to check if a delegate is a multicast delegate.
     * @tparam T The type to check if it's a delegate or not
     */
    template <typename T>
    concept MulticastDelegate = NativeMulitcastDelegate<T> || DynamicMulitcastDelegate<T>;

    template <typename T>
    concept NativeDelegate = NativeUnicastDelegate<T> || NativeMulitcastDelegate<T>;

    template <typename T>
    concept DynamicDelegate = DynamicUnicastDelegate<T> || DynamicMulitcastDelegate<T>;

    /**
     * Concept to check if a delegate is a Unreal Engine delegate type
     * @tparam T The type to check if it's a delegate or not
     */
    template <typename T>
    concept UEDelegate = UnicastDelegate<T> || MulticastDelegate<T>;

    /**
     * Concept to check if a single-cast delegate is bindable to a target multicast delegate
     */
    template <typename M, typename S>
    concept BindableTo = MulticastDelegate<M> && UnicastDelegate<S> && requires(M &&Delegate, S &&Source) {
        { Delegate.Add(std::forward<S>(Source)) } -> std::same_as<FDelegateHandle>;
    };

    template <typename F, typename T>
    concept MemberFunctionOf = StdExt::IsMemberFunction_v<F> && std::is_same_v<StdExt::MemberFunctionClass_t<F>, T>;

    template <typename F, typename T>
    concept UObjectMember = std::derived_from<T, UObject> && MemberFunctionOf<F, T>;

    template <typename D, typename F, typename... A>
    concept CanBindStatic = NativeUnicastDelegate<D> && requires(D &&Delegate, F &&Functor, A &&...Args) {
        Delegate.BindStatic(std::forward<F>(Functor), std::forward<A>(Args)...);
        { D::CreateStatic(std::forward<F>(Functor), std::forward<A>(Args)...) } -> std::same_as<std::remove_cvref_t<D>>;
    };

    template <typename D, typename F, typename... A>
    concept CanBindLambda = NativeUnicastDelegate<D> && requires(D &&Delegate, F &&Functor, A &&...Args) {
        Delegate.BindLambda(std::forward<F>(Functor), std::forward<A>(Args)...);
        { D::CreateLambda(std::forward<F>(Functor), std::forward<A>(Args)...) } -> std::same_as<std::remove_cvref_t<D>>;
    };

    template <typename D, typename O, typename F, typename... A>
    concept CanBindRaw = NativeUnicastDelegate<D> && MemberFunctionOf<F, O> &&
                         requires(D &&Delegate, O *Object, F &&Functor, A &&...Args) {
                             Delegate.BindRaw(Object, std::forward<F>(Functor), std::forward<A>(Args)...);
                             {
                                 D::CreateRaw(Object, std::forward<F>(Functor), std::forward<A>(Args)...)
                             } -> std::same_as<std::remove_cvref_t<D>>;
                         };

    template <typename D, typename O, ESPMode M, typename F, typename... A>
    concept CanBindSP = NativeUnicastDelegate<D> && MemberFunctionOf<F, O> &&
                        requires(D &&Delegate, const TSharedRef<O, M> &Object, F &&Functor, A &&...Args) {
                            Delegate.BindSP(Object, std::forward<F>(Functor), std::forward<A>(Args)...);
                            {
                                D::CreateSP(Object, std::forward<F>(Functor), std::forward<A>(Args)...)
                            } -> std::same_as<std::remove_cvref_t<D>>;
                        };

    template <typename D, typename O, ESPMode M, typename F, typename... A>
    concept CanBindSPLambda = NativeUnicastDelegate<D> && MemberFunctionOf<F, O> &&
                              requires(D &&Delegate, const TSharedRef<O, M> &Object, F &&Functor, A &&...Args) {
                                  Delegate.BindSPLambda(Object, std::forward<F>(Functor), std::forward<A>(Args)...);
                                  {
                                      D::CreateSPLambda(Object, std::forward<F>(Functor), std::forward<A>(Args)...)
                                  } -> std::same_as<std::remove_cvref_t<D>>;
                              };

    template <typename D, typename O, typename F, typename... A>
    concept CanBindUObject =
        NativeUnicastDelegate<D> && UObjectMember<F, O> && requires(D &&Delegate, O *Object, F &&Functor, A &&...Args) {
            Delegate.BindUObject(Object, std::forward<F>(Functor), std::forward<A>(Args)...);
            {
                D::CreateUObject(Object, std::forward<F>(Functor), std::forward<A>(Args)...)
            } -> std::same_as<std::remove_cvref_t<D>>;
        };

    template <typename D, typename O, typename F, typename... A>
    concept CanBindWeakLambda = NativeUnicastDelegate<D> && std::is_base_of_v<UObject, O> &&
                                requires(D &&Delegate, O *Object, F &&Functor, A &&...Args) {
                                    Delegate.BindWeakLambda(Object, std::forward<F>(Functor), std::forward<A>(Args)...);
                                    {
                                        D::CreateWeakLambda(Object, std::forward<F>(Functor), std::forward<A>(Args)...)
                                    } -> std::same_as<std::remove_cvref_t<D>>;
                                };

    template <typename D, typename F, typename... A>
    concept CanAddStatic = NativeMulitcastDelegate<D> && requires(D &&Delegate, F &&Functor, A &&...Args) {
        { Delegate.AddStatic(std::forward<F>(Functor), std::forward<A>(Args)...) } -> std::same_as<FDelegateHandle>;
    };

    template <typename D, typename F, typename... A>
    concept CanAddLambda = NativeMulitcastDelegate<D> && requires(D &&Delegate, F &&Functor, A &&...Args) {
        { Delegate.AddLambda(std::forward<F>(Functor), std::forward<A>(Args)...) } -> std::same_as<FDelegateHandle>;
    };

    template <typename D, typename O, typename F, typename... A>
    concept CanAddRaw = NativeMulitcastDelegate<D> && MemberFunctionOf<F, O> &&
                        requires(D &&Delegate, O *Object, F &&Functor, A &&...Args) {
                            {
                                Delegate.AddRaw(Object, std::forward<F>(Functor), std::forward<A>(Args)...)
                            } -> std::same_as<FDelegateHandle>;
                        };

    template <typename D, typename O, ESPMode M, typename F, typename... A>
    concept CanAddSP = NativeMulitcastDelegate<D> && MemberFunctionOf<F, O> &&
                       requires(D &&Delegate, const TSharedRef<O, M> &Object, F &&Functor, A &&...Args) {
                           {
                               Delegate.AddSP(Object, std::forward<F>(Functor), std::forward<A>(Args)...)
                           } -> std::same_as<FDelegateHandle>;
                       };

    template <typename D, typename O, ESPMode M, typename F, typename... A>
    concept CanAddSPLambda = NativeMulitcastDelegate<D> && MemberFunctionOf<F, O> &&
                             requires(D &&Delegate, const TSharedRef<O, M> &Object, F &&Functor, A &&...Args) {
                                 {
                                     Delegate.AddSPLambda(Object, std::forward<F>(Functor), std::forward<A>(Args)...)
                                 } -> std::same_as<FDelegateHandle>;
                             };

    template <typename D, typename O, typename F, typename... A>
    concept CanAddUObject = NativeMulitcastDelegate<D> && UObjectMember<F, O> &&
                            requires(D &&Delegate, O *Object, F &&Functor, A &&...Args) {
                                {
                                    Delegate.AddUObject(Object, std::forward<F>(Functor), std::forward<A>(Args)...)
                                } -> std::same_as<FDelegateHandle>;
                            };

    template <typename D, typename O, typename F, typename... A>
    concept CanAddWeakLambda =
        NativeMulitcastDelegate<D> && std::is_base_of_v<UObject, O> &&
        requires(D &&Delegate, O *Object, F &&Functor, A &&...Args) {
            {
                Delegate.AddWeakLambda(Object, std::forward<F>(Functor), std::forward<A>(Args)...)
            } -> std::same_as<FDelegateHandle>;
        };

    namespace Detail {
        template <typename T>
        struct TDelegateTraits;

        template <typename T>
            requires UnicastDelegate<T>
        struct TDelegateTraits<T> {
            using InvokeFunctionSignature = decltype(&T::Execute);
        };

        template <typename T>
            requires MulticastDelegate<T>
        struct TDelegateTraits<T> {
            using InvokeFunctionSignature = decltype(&T::Broadcast);
        };
    } // namespace Detail

    template <typename T>
        requires UEDelegate<T>
    using TDelegateReturnType = StdExt::ReturnType_t<typename Detail::TDelegateTraits<T>::InvokeFunctionSignature>;

    template <typename T>
        requires UEDelegate<T>
    using TDelegateArgTypes = StdExt::ArgTypes_t<typename Detail::TDelegateTraits<T>::InvokeFunctionSignature>;

} // namespace UE::Ranges