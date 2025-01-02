// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#ifdef __UNREAL__
#include "RetroLib/Concepts/Delegates.h"
#include "RetroLib/Functional/CreateBinding.h"
#include "RetroLib/Functional/ExtensionMethods.h"

#ifndef RETROLIB_EXPORT
#define RETROLIB_EXPORT
#endif

namespace Retro {
    namespace Delegates
    {
        template <UEDelegate D>
        struct TDelegateInvoker {

            template <typename T>
                requires std::constructible_from<D, T> && (!std::same_as<std::decay_t<T>, TDelegateInvoker>)
            constexpr explicit TDelegateInvoker(T &&Delegate) : Delegate(std::forward<T>(Delegate)) {
            }

            template <typename... A>
            constexpr decltype(auto) operator()(A &&... Args) const {
                if constexpr (UnicastDelegate<D>) {
                    check(Delegate.IsBound())
                    return Delegate.Execute(std::forward<A>(Args)...);
                } else if constexpr (MulticastDelegate<D>) {
                    return Delegate.Broadcast(std::forward<A>(Args)...);
                }
            }

        private:
            D Delegate;
        };

        template <UEDelegate D>
        TDelegateInvoker(D &&) -> TDelegateInvoker<std::decay_t<D>>;
    }

    RETROLIB_EXPORT template <Delegates::UEDelegate D>
    struct TAdditionalBindingTypes<D> : FValidType {
        template <Delegates::UEDelegate F, typename... A>
            requires std::same_as<D, std::decay_t<F>>
        static constexpr auto Bind(F &&Delegate, A &&... Args) {
            return Retro::BindBack(Delegates::TDelegateInvoker(std::forward<F>(Delegate)), std::forward<A>(Args)...);
        }
    };

    namespace Delegates {
        RETROLIB_EXPORT template <UEDelegate D, typename F, typename... A>
            requires CanBindFree<D, F, A...>
        D Create(F &&Functor, A &&... Args) {
            if constexpr (CanBindStatic<D, F, A...>) {
                return D::CreateStatic(std::forward<F>(Functor), std::forward<A>(Args)...);
            } else {
                static_assert(CanBindLambda<D, F, A...>);
                return D::CreateLambda(std::forward<F>(Functor), std::forward<A>(Args)...);
            }
        }

        RETROLIB_EXPORT template <UEDelegate D, typename O, typename F, typename... A>
            requires CanBindMember<D, O, F, A...>
        D Create(O &&Object, F &&Functor, A &&... Args) {
            if constexpr (CanBindSP<D, O, F, A...>) {
                return D::CreateSP(std::forward<O>(Object), std::forward<F>(Functor), std::forward<A>(Args)...);
            } else if constexpr (CanBindSPLambda<D, O, F, A...>) {
                return D::CreateSPLambda(std::forward<O>(Object), std::forward<F>(Functor), std::forward<A>(Args)...);
            } else if constexpr (CanBindUObject<D, O, F, A...>) {
                return D::CreateUObject(std::forward<O>(Object), std::forward<F>(Functor), std::forward<A>(Args)...);
            } else if constexpr (CanBindWeakLambda<D, O, F, A...>) {
                return D::CreateWeakLambda(std::forward<O>(Object), std::forward<F>(Functor), std::forward<A>(Args)...);
            } else {
                static_assert(CanBindRaw<D, O, F, A...>);
                return D::CreateRaw(std::forward<O>(Object), std::forward<F>(Functor), std::forward<A>(Args)...);
            }
        }

        struct FDelegateBinder {
            template <NativeUnicastDelegate D, typename F, typename... A>
                requires CanBindFree<D, F, A...>
            void operator()(D &Delegate, F &&Functor, A &&... Args) const {
                if constexpr (CanBindStatic<D, F, A...>) {
                    return Delegate.BindStatic(std::forward<F>(Functor), std::forward<A>(Args)...);
                } else {
                    static_assert(CanBindLambda<D, F, A...>);
                    return Delegate.BindLambda(std::forward<F>(Functor), std::forward<A>(Args)...);
                }
            }

            template <NativeUnicastDelegate D, typename O, typename F, typename... A>
                requires CanBindMember<D, O, F, A...>
            void operator()(D &Delegate, O &&Object, F &&Functor, A &&... Args) const {
                if constexpr (CanBindSP<D, O, F, A...>) {
                    return Delegate.BindSP(std::forward<O>(Object), std::forward<F>(Functor), std::forward<A>(Args)...);
                } else if constexpr (CanBindSPLambda<D, O, F, A...>) {
                    return Delegate.BindSPLambda(std::forward<O>(Object), std::forward<F>(Functor),
                                                 std::forward<A>(Args)...);
                } else if constexpr (CanBindUObject<D, O, F, A...>) {
                    return Delegate.BindUObject(std::forward<O>(Object), std::forward<F>(Functor),
                                                std::forward<A>(Args)...);
                } else if constexpr (CanBindWeakLambda<D, O, F, A...>) {
                    return Delegate.BindWeakLambda(std::forward<O>(Object), std::forward<F>(Functor),
                                                   std::forward<A>(Args)...);
                } else {
                    static_assert(CanBindRaw<D, O, F, A...>);
                    return Delegate.BindRaw(std::forward<O>(Object), std::forward<F>(Functor), std::forward<A>(Args)...);
                }
            }
        };

        RETROLIB_EXPORT constexpr auto Bind = ExtensionMethod<FDelegateBinder{}>;

        struct FDelegateAdder {
            template <MulticastDelegate D, UnicastDelegate O>
                requires BindableTo<D, O>
            decltype(auto) operator()(D& Delegate, O &&Binding) const {
                return Delegate.Add(std::forward<O>(Binding));
            }
            
            template <NativeMulitcastDelegate D, typename F, typename... A>
                requires CanAddFree<D, F, A...>
            FDelegateHandle operator()(D &Delegate, F &&Functor, A &&... Args) const {
                if constexpr (CanAddStatic<D, F, A...>) {
                    return Delegate.AddStatic(std::forward<F>(Functor), std::forward<A>(Args)...);
                } else {
                    static_assert(CanAddLambda<D, F, A...>);
                    return Delegate.AddLambda(std::forward<F>(Functor), std::forward<A>(Args)...);
                }
            }

            template <NativeMulitcastDelegate D, typename O, typename F, typename... A>
                requires CanAddMember<D, O, F, A...>
            FDelegateHandle operator()(D &Delegate, O &&Object, F &&Functor, A &&... Args) const {
                if constexpr (CanAddSP<D, O, F, A...>) {
                    return Delegate.AddSP(std::forward<O>(Object), std::forward<F>(Functor), std::forward<A>(Args)...);
                } else if constexpr (CanAddSPLambda<D, O, F, A...>) {
                    return Delegate.AddSPLambda(std::forward<O>(Object), std::forward<F>(Functor),
                                                 std::forward<A>(Args)...);
                } else if constexpr (CanAddUObject<D, O, F, A...>) {
                    return Delegate.AddUObject(std::forward<O>(Object), std::forward<F>(Functor),
                                                std::forward<A>(Args)...);
                } else if constexpr (CanAddWeakLambda<D, O, F, A...>) {
                    return Delegate.AddWeakLambda(std::forward<O>(Object), std::forward<F>(Functor),
                                                   std::forward<A>(Args)...);
                } else {
                    static_assert(CanAddRaw<D, O, F, A...>);
                    return Delegate.AddRaw(std::forward<O>(Object), std::forward<F>(Functor), std::forward<A>(Args)...);
                }
            }
        };

        RETROLIB_EXPORT constexpr auto Add = ExtensionMethod<FDelegateAdder{}>;
    }

} // namespace retro
#endif