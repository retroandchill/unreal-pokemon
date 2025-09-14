// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#ifdef __UNREAL__
#include "RetroLib/Concepts/Delegates.h"
#include "RetroLib/Functional/CreateBinding.h"
#include "RetroLib/Functional/ExtensionMethods.h"

#ifndef RETROLIB_EXPORT
#define RETROLIB_EXPORT
#endif

namespace Retro
{
    namespace Delegates
    {
        template <UEDelegate D>
        struct TDelegateInvoker
        {

            template <typename T>
                requires std::constructible_from<D, T> && (!std::same_as<std::decay_t<T>, TDelegateInvoker>)
            constexpr explicit TDelegateInvoker(T &&Delegate) : Delegate(std::forward<T>(Delegate))
            {
            }

            template <typename... A>
            constexpr decltype(auto) operator()(A &&...Args) const
            {
                if constexpr (UnicastDelegate<D>)
                {
                    check(Delegate.IsBound())
                    return Delegate.Execute(std::forward<A>(Args)...);
                }
                else if constexpr (MulticastDelegate<D>)
                {
                    return Delegate.Broadcast(std::forward<A>(Args)...);
                }
            }

          private:
            D Delegate;
        };

        template <UEDelegate D>
        TDelegateInvoker(D &&) -> TDelegateInvoker<std::decay_t<D>>;
    } // namespace Delegates

    RETROLIB_EXPORT template <Delegates::UEDelegate D, typename... A>
    constexpr auto BindDelegate(D &&Delegate, A &&...Args)
    {
        if constexpr (sizeof...(A) > 0)
        {
            return BindBack(Delegates::TDelegateInvoker(std::forward<D>(Delegate)), std::forward<A>(Args)...);
        }
        else
        {
            return Delegates::TDelegateInvoker(std::forward<D>(Delegate));
        }
    }

    RETROLIB_EXPORT template <Delegates::UEDelegate D>
    struct TAdditionalBindingTypes<D> : FValidType
    {
        template <Delegates::UEDelegate F, typename... A>
            requires std::same_as<D, std::decay_t<F>>
        static constexpr auto Bind(F &&Delegate, A &&...Args)
        {
            return Retro::BindBack(Delegates::TDelegateInvoker(std::forward<F>(Delegate)), std::forward<A>(Args)...);
        }
    };

    namespace Delegates
    {
        RETROLIB_EXPORT template <UEDelegate D, typename F, typename... A>
            requires CanBindFree<D, F, A...>
        D Create(F &&Functor, A &&...Args)
        {
            if constexpr (CanBindStatic<D, F, A...>)
            {
                return D::CreateStatic(std::forward<F>(Functor), std::forward<A>(Args)...);
            } // namespace Delegates
            else
            {
                static_assert(CanBindLambda<D, F, A...>);
                return D::CreateLambda(std::forward<F>(Functor), std::forward<A>(Args)...);
            }
        } // namespace Delegates

        RETROLIB_EXPORT template <UEDelegate D, typename O, typename F, typename... A>
            requires CanBindMember<D, O, F, A...>
        D Create(O &&Object, F &&Functor, A &&...Args)
        {
            if constexpr (CanBindSP<D, O, F, A...>)
            {
                return D::CreateSP(std::forward<O>(Object), std::forward<F>(Functor), std::forward<A>(Args)...);
            }
            else if constexpr (CanBindSPLambda<D, O, F, A...>)
            {
                return D::CreateSPLambda(std::forward<O>(Object), std::forward<F>(Functor), std::forward<A>(Args)...);
            }
            else if constexpr (CanBindUObject<D, O, F, A...>)
            {
                return D::CreateUObject(std::forward<O>(Object), std::forward<F>(Functor), std::forward<A>(Args)...);
            }
            else if constexpr (CanBindWeakLambda<D, O, F, A...>)
            {
                return D::CreateWeakLambda(std::forward<O>(Object), std::forward<F>(Functor), std::forward<A>(Args)...);
            }
            else
            {
                static_assert(CanBindRaw<D, O, F, A...>);
                return D::CreateRaw(std::forward<O>(Object), std::forward<F>(Functor), std::forward<A>(Args)...);
            }
        }

        struct FDelegateBinder
        {
            template <NativeUnicastDelegate D, typename F, typename... A>
                requires CanBindFree<D, F, A...>
            void operator()(D &Delegate, F &&Functor, A &&...Args) const
            {
                if constexpr (CanBindStatic<D, F, A...>)
                {
                    return Delegate.BindStatic(std::forward<F>(Functor), std::forward<A>(Args)...);
                }
                else
                {
                    static_assert(CanBindLambda<D, F, A...>);
                    return Delegate.BindLambda(std::forward<F>(Functor), std::forward<A>(Args)...);
                }
            }

            template <NativeUnicastDelegate D, typename O, typename F, typename... A>
                requires CanBindMember<D, O, F, A...>
            void operator()(D &Delegate, O &&Object, F &&Functor, A &&...Args) const
            {
                if constexpr (CanBindSP<D, O, F, A...>)
                {
                    return Delegate.BindSP(std::forward<O>(Object), std::forward<F>(Functor), std::forward<A>(Args)...);
                }
                else if constexpr (CanBindSPLambda<D, O, F, A...>)
                {
                    return Delegate.BindSPLambda(std::forward<O>(Object), std::forward<F>(Functor),
                                                 std::forward<A>(Args)...);
                }
                else if constexpr (CanBindUObject<D, O, F, A...>)
                {
                    return Delegate.BindUObject(std::forward<O>(Object), std::forward<F>(Functor),
                                                std::forward<A>(Args)...);
                }
                else if constexpr (CanBindWeakLambda<D, O, F, A...>)
                {
                    return Delegate.BindWeakLambda(std::forward<O>(Object), std::forward<F>(Functor),
                                                   std::forward<A>(Args)...);
                }
                else
                {
                    static_assert(CanBindRaw<D, O, F, A...>);
                    return Delegate.BindRaw(std::forward<O>(Object), std::forward<F>(Functor),
                                            std::forward<A>(Args)...);
                }
            }
        };

        RETROLIB_EXPORT constexpr auto Bind = ExtensionMethod<FDelegateBinder{}>;

        template <typename D, typename... A>
        concept CanBindToDelegate = requires(D &Delegate, A &&...Args) {
            { Retro::Delegates::Bind(Delegate, std::forward<A>(Args)...) } -> std::same_as<FDelegateHandle>;
        };

        struct FDelegateAdder
        {
            template <MulticastDelegate D, UnicastDelegate O>
                requires BindableTo<D, O>
            decltype(auto) operator()(D &Delegate, O &&Binding) const
            {
                return Delegate.Add(std::forward<O>(Binding));
            }

            template <NativeMulitcastDelegate D, typename F, typename... A>
                requires CanAddFree<D, F, A...>
            FDelegateHandle operator()(D &Delegate, F &&Functor, A &&...Args) const
            {
                if constexpr (CanAddStatic<D, F, A...>)
                {
                    return Delegate.AddStatic(std::forward<F>(Functor), std::forward<A>(Args)...);
                }
                else
                {
                    static_assert(CanAddLambda<D, F, A...>);
                    return Delegate.AddLambda(std::forward<F>(Functor), std::forward<A>(Args)...);
                }
            }

            template <NativeMulitcastDelegate D, typename O, typename F, typename... A>
                requires CanAddMember<D, O, F, A...>
            FDelegateHandle operator()(D &Delegate, O &&Object, F &&Functor, A &&...Args) const
            {
                if constexpr (CanAddSP<D, O, F, A...>)
                {
                    return Delegate.AddSP(std::forward<O>(Object), std::forward<F>(Functor), std::forward<A>(Args)...);
                }
                else if constexpr (CanAddSPLambda<D, O, F, A...>)
                {
                    return Delegate.AddSPLambda(std::forward<O>(Object), std::forward<F>(Functor),
                                                std::forward<A>(Args)...);
                }
                else if constexpr (CanAddUObject<D, O, F, A...>)
                {
                    return Delegate.AddUObject(std::forward<O>(Object), std::forward<F>(Functor),
                                               std::forward<A>(Args)...);
                }
                else if constexpr (CanAddWeakLambda<D, O, F, A...>)
                {
                    return Delegate.AddWeakLambda(std::forward<O>(Object), std::forward<F>(Functor),
                                                  std::forward<A>(Args)...);
                }
                else
                {
                    static_assert(CanAddRaw<D, O, F, A...>);
                    return Delegate.AddRaw(std::forward<O>(Object), std::forward<F>(Functor), std::forward<A>(Args)...);
                }
            }
        };

        RETROLIB_EXPORT constexpr auto Add = ExtensionMethod<FDelegateAdder{}>;

        template <typename D, typename... A>
        concept CanAddToDelegate = requires(D &Delegate, A &&...Args) {
            { Retro::Delegates::Add(Delegate, std::forward<A>(Args)...) } -> std::same_as<FDelegateHandle>;
        };

        /**
         * @class TScopedBinding
         * @brief A RAII class for managing temporary bindings to native delegates.
         *
         * TScopedBinding is a utility that automatically binds a callable to a native
         * delegate and ensures the binding is released when the instance of this class
         * goes out of scope.
         *
         * This class is templated based on the type of the native delegate.
         *
         * There are different specializations of this class for native unicast
         * delegates and native multicast delegates.
         *
         * @tparam D The type of the native delegate. It must satisfy the NativeDelegate concept.
         *
         * @note Instances of this class are non-copyable.
         */
        RETROLIB_EXPORT template <NativeDelegate>
        class TScopedBinding;

        /**
         * @class TScopedBinding
         * @brief A RAII (Resource Acquisition Is Initialization) wrapper for managing delegate bindings.
         *
         * The TScopedBinding class automatically binds a specified delegate with the provided arguments
         * upon construction and ensures the delegate is unbound when the TScopedBinding instance
         * goes out of scope. This provides safe and automatic cleanup, preventing dangling delegate
         * bindings.
         *
         * @tparam D The type of the delegate to bind. It must satisfy the CanBindToDelegate concept.
         *
         * @note This class is non-copyable to ensure proper binding and unbinding semantics.
         */
        RETROLIB_EXPORT template <NativeUnicastDelegate D>
        class TScopedBinding<D>
        {
          public:
            /**
             * @brief Constructs a TScopedBinding instance and binds the provided delegate with the specified arguments.
             *
             * On construction, this constructor binds the given delegate instance by forwarding the provided arguments
             * to the delegate's binding mechanism. This ensures that the delegate is properly bound during the
             * lifespan of the TScopedBinding instance.
             *
             * @param Delegate Reference to the delegate to be managed.
             * @param Args Variadic template arguments to be forwarded to the delegate binding process.
             *
             * @tparam A Parameter pack representing the types of the arguments to bind the delegate.
             *
             * @note The delegate binding mechanism uses the `Bind` function internally to establish the connection.
             *       The binding will automatically be unbound when the TScopedBinding instance is destroyed.
             */
            template <typename... A>
                requires CanBindToDelegate<D, A...>
            constexpr explicit TScopedBinding(D &Delegate, A &&...Args) : Delegate(Delegate)
            {
                Bind(Delegate, std::forward<A>(Args)...);
            }

            /**
             * @brief Destructor for TScopedBinding.
             *
             * Automatically unbinds the managed delegate when the TScopedBinding instance is destroyed.
             *
             * This ensures that the delegate is properly cleaned up when the TScopedBinding object goes
             * out of scope, helping to manage delegate lifetimes and prevent unintended behavior due to
             * dangling bindings.
             *
             * @note The destructor calls the `Unbind` function on the managed delegate to release the binding.
             */
            ~TScopedBinding()
            {
                Delegate.Unbind();
            }

            UE_NONCOPYABLE(TScopedBinding)

          private:
            D &Delegate;
        };

        /**
         * @class TScopedBinding
         * @brief A RAII (Resource Acquisition Is Initialization) utility for managing delegate bindings.
         *
         * The TScopedBinding class ensures that a delegate binding is automatically established during
         * construction and released when the instance goes out of scope. This helps to maintain safe
         * and predictable lifecycle management of delegate bindings, particularly in scenarios where
         * temporary bindings are required.
         *
         * @tparam D The type of the delegate to manage. It must satisfy the CanAddToDelegate concept.
         *
         * @note This class is non-copyable to enforce proper binding and unbinding semantics.
         */
        RETROLIB_EXPORT template <NativeMulitcastDelegate D>
        class TScopedBinding<D>
        {
          public:
            /**
             * @brief Constructs a TScopedBinding instance, automatically binding a delegate.
             *
             * This constructor initializes the TScopedBinding object by binding the specified
             * delegate with the provided arguments. The binding is performed using the `Add`
             * function, and ensures that the delegate is properly connected as part of the
             * TScopedBinding's lifecycle. When the TScopedBinding instance is destroyed,
             * the binding will be automatically released.
             *
             * @param Delegate The delegate to which a binding is added. This must satisfy the CanAddToDelegate concept.
             * @param Args Arguments to be passed for binding the delegate. These are forwarded to the `Add` function.
             */
            template <typename... A>
                requires CanAddToDelegate<D, A...>
            constexpr explicit TScopedBinding(D &Delegate, A &&...Args)
                : Delegate(Delegate), DelegateHandle(Add(Delegate, std::forward<A>(Args)...))
            {
            }

            /**
             * @brief Destructor for TScopedBinding.
             *
             * Automatically removes the binding associated with the managed delegate
             * when the TScopedBinding instance is destroyed.
             *
             * Ensures that the delegate binding is properly cleaned up, preventing
             * issues such as dangling references or unintentional behavior from
             * lingering bindings.
             *
             * @note The destructor calls the `Remove` function on the delegate with
             * the stored delegate handle to release the binding.
             */
            ~TScopedBinding()
            {
                Delegate.Remove(DelegateHandle);
            }

            UE_NONCOPYABLE(TScopedBinding)

          private:
            D &Delegate;
            FDelegateHandle DelegateHandle;
        };

        template <NativeDelegate D, typename... A>
        TScopedBinding(D &, A &&...) -> TScopedBinding<D>;
    } // namespace Delegates

} // namespace Retro
#endif