// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Ranges/Concepts/Delegates.h"
#include "Ranges/Concepts/UObjectPointer.h"

namespace UE::Ranges {
    template <typename D, typename F, typename... A>
        requires CanBindStatic<D, F, A...> || CanBindLambda<D, F, A...>
    D CreateDelegate(F &&Functor, A &&...Args) {
        if constexpr (CanBindStatic<D, F, A...>) {
            return D::CreateStatic(std::forward<F>(Functor), std::forward<A>(Args)...);
        } else {
            return D::CreateLambda(std::forward<F>(Functor), std::forward<A>(Args)...);
        }
    }

    template <typename D, typename T, typename F, typename... A>
        requires CanBindUObject<D, T, F, A...> || CanBindWeakLambda<D, T, F, A...> || CanBindRaw<D, T, F, A...>
    D CreateDelegate(T *Object, F &&Functor, A &&...Args) {
        if constexpr (CanBindUObject<D, T, F, A...>) {
            return D::CreateUObject(Object, std::forward<F>(Functor), std::forward<A>(Args)...);
        } else if constexpr (StdExt::IsMemberFunction_v<F> && std::derived_from<T, StdExt::MemberFunctionClass_t<F>>) {
            return D::CreateWeakLambda(Object, CreateBinding(Object, std::forward<F>(Functor), std::forward<A>(Args)...));
        } else if constexpr (CanBindWeakLambda<D, T, F, A...>) {
            return D::CreateWeakLambda(Object, std::forward<F>(Functor), std::forward<A>(Args)...);
        } else {
            return D::CreateRaw(Object, std::forward<F>(Functor), std::forward<A>(Args)...);
        }
    }

    template <typename D, typename T, ESPMode M, typename F, typename... A>
        requires CanBindSP<D, T, M, F, A...> || CanBindSPLambda<D, T, M, F, A...>
    D CreateDelegate(const TSharedRef<T, M> &Object, F &&Functor, A &&...Args) {
        if constexpr (CanBindSP<D, T, M, F, A...>) {
            return D::CreateSP(Object, std::forward<F>(Functor), std::forward<A>(Args)...);
        } else {
            return D::CreateSPLambda(Object, std::forward<F>(Functor), std::forward<A>(Args)...);
        }
    }

    template <typename D, typename T, typename... A>
        requires NativeUnicastDelegate<D> && UObjectPointer<T>
    D CreateDelegate(T &&Object, FName FunctionName, A &&...Args) {
        return D::CreateUFunction(std::forward<T>(Object), FunctionName, std::forward<A>(Args)...);
    }

    template <typename D>
        requires UnicastDelegate<D>
    void BindToDelegate(D &Delegate, D &&Binding) {
        Delegate = std::forward<D>(Binding);
    }

    template <typename D, typename F, typename... A>
        requires CanBindStatic<D, F, A...> || CanBindLambda<D, F, A...>
    void BindToDelegate(D &Delegate, F &&Functor, A &&...Args) {
        if constexpr (CanBindStatic<D, F, A...>) {
            Delegate.BindStatic(std::forward<F>(Functor), std::forward<A>(Args)...);
        } else {
            Delegate.BindLambda(std::forward<F>(Functor), std::forward<A>(Args)...);
        }
    }

    template <typename D, typename T, typename F, typename... A>
        requires CanBindUObject<D, T, F, A...> || CanBindWeakLambda<D, T, F, A...> || CanBindRaw<D, T, F, A...>
    void BindToDelegate(D &Delegate, T *Object, F &&Functor, A &&...Args) {
        if constexpr (CanBindUObject<D, T, F, A...>) {
            Delegate.BindUObject(Object, std::forward<F>(Functor), std::forward<A>(Args)...);
        } else if constexpr (StdExt::IsMemberFunction_v<F> && std::derived_from<T, StdExt::MemberFunctionClass_t<F>>) {
            Delegate.BindWeakLambda(Object, CreateBinding(Object, std::forward<F>(Functor), std::forward<A>(Args)...));
        } else if constexpr (CanBindWeakLambda<D, T, F, A...>) {
            Delegate.BindWeakLambda(Object, std::forward<F>(Functor), std::forward<A>(Args)...);
        } else {
            Delegate.BindRaw(Object, std::forward<F>(Functor), std::forward<A>(Args)...);
        }
    }

    template <typename D, typename T, ESPMode M, typename F, typename... A>
        requires CanBindSP<D, T, M, F, A...> || CanBindSPLambda<D, T, M, F, A...>
    void BindToDelegate(D &Delegate, const TSharedRef<T, M> &Object, F &&Functor, A &&...Args) {
        if constexpr (CanBindSP<D, T, M, F, A...>) {
            Delegate.BindSP(Object, std::forward<F>(Functor), std::forward<A>(Args)...);
        } else {
            Delegate.BindSPLambda(Object, std::forward<F>(Functor), std::forward<A>(Args)...);
        }
    }

    template <typename D, typename T, typename... A>
        requires NativeUnicastDelegate<D> && UObjectPointer<T>
    void BindToDelegate(D &Delegate, T &&Object, FName FunctionName, A &&...Args) {
        Delegate.BindUFunction(std::forward<T>(Object), FunctionName, std::forward<A>(Args)...);
    }

    template <typename D, typename... A>
    concept CanBindDelegate =
        requires(D &&Delegate, A &&...Args) { BindToDelegate<D, A...>(std::forward<D>(), std::forward<A>(Args)...); };

    template <typename M, typename U>
        requires MulticastDelegate<M> && BindableTo<M, U>
    FDelegateHandle AddToDelegate(M &Delegate, U &&Binding) {
        return Delegate.Add(std::forward<U>(Binding));
    }

    template <typename D, typename F, typename... A>
        requires CanAddStatic<D, F, A...> || CanAddLambda<D, F, A...>
    FDelegateHandle AddToDelegate(D &Delegate, F &&Functor, A &&...Args) {
        if constexpr (CanAddStatic<D, F, A...>) {
            return Delegate.AddStatic(std::forward<F>(Functor), std::forward<A>(Args)...);
        } else {
            return Delegate.AddLambda(std::forward<F>(Functor), std::forward<A>(Args)...);
        }
    }

    template <typename D, typename T, typename F, typename... A>
        requires CanAddUObject<D, T, F, A...> || CanAddWeakLambda<D, T, F, A...> || CanAddRaw<D, T, F, A...>
    FDelegateHandle AddToDelegate(D &Delegate, T *Object, F &&Functor, A &&...Args) {
        if constexpr (CanAddUObject<D, T, F, A...>) {
            return Delegate.AddUObject(Object, std::forward<F>(Functor), std::forward<A>(Args)...);
        } else if constexpr (StdExt::IsMemberFunction_v<F> && std::derived_from<T, StdExt::MemberFunctionClass_t<F>>) {
            return Delegate.AddWeakLambda(Object, CreateBinding(Object, std::forward<F>(Functor), std::forward<A>(Args)...));
        } else if constexpr (CanAddWeakLambda<D, T, F, A...>) {
            return Delegate.AddWeakLambda(Object, std::forward<F>(Functor), std::forward<A>(Args)...);
        } else {
            return Delegate.AddRaw(Object, std::forward<F>(Functor), std::forward<A>(Args)...);
        }
    }

    template <typename D, typename T, ESPMode M, typename F, typename... A>
        requires CanAddSP<D, T, M, F, A...> || CanAddSPLambda<D, T, M, F, A...>
    FDelegateHandle AddToDelegate(D &Delegate, const TSharedRef<T, M> &Object, F &&Functor, A &&...Args) {
        if constexpr (CanBindSP<D, T, M, F, A...>) {
            return Delegate.AddSP(Object, std::forward<F>(Functor), std::forward<A>(Args)...);
        } else {
            return Delegate.AddSPLambda(Object, std::forward<F>(Functor), std::forward<A>(Args)...);
        }
    }

    template <typename D, typename T, typename... A>
        requires NativeMulitcastDelegate<D> && UObjectPointer<T>
    FDelegateHandle AddToDelegate(D &Delegate, T &&Object, FName FunctionName, A &&...Args) {
        return Delegate.AddUFunction(std::forward<T>(Object), FunctionName, std::forward<A>(Args)...);
    }

    template <typename D, typename... A>
    concept CanAddToDelegate = NativeMulitcastDelegate<D> && requires(D &Delegate, A &&...Args) {
        { AddToDelegate(Delegate, std::forward<A>(Args)...) } -> std::same_as<FDelegateHandle>;
    };
} // namespace UE::Ranges

/**
 * Declares a multicast delegate member for a class adding some basic boilerplate methods that are used to bind the
 * delegate and release the binding, but allowing the invocation to remain private.
 * @param DelegateType The type of delegate
 * @param MemberName The name of the member variable
 */
#define UE_MULTICAST_DELEGATE_MEMBER(DelegateType, MemberName)                                                         \
  private:                                                                                                             \
    DelegateType MemberName;                                                                                           \
                                                                                                                       \
  public:                                                                                                              \
    template <typename... A>                                                                                           \
        requires UE::Ranges::CanAddToDelegate<DelegateType, A...>                                                      \
    FDelegateHandle BindTo##MemberName(A &&...Args) {                                                                  \
        return UE::Ranges::AddToDelegate(MemberName, std::forward<A>(Args)...);                                        \
    }                                                                                                                  \
    void RemoveFrom##MemberName(FDelegateHandle Handle) {                                                              \
        MemberName.Remove(Handle);                                                                                     \
    }