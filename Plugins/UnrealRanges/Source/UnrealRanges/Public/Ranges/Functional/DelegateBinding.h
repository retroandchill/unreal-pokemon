// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Ranges/Concepts/Delegates.h"
#include "Ranges/RangeConcepts.h"
#include <TypeTraits.h>

namespace UE::Ranges {

    template <typename D, typename... A>
        requires UEDelegate<D>
    constexpr auto InvokeDelegate(D &&Delegate, A &&...Args) {
        if constexpr (SingleBindDelegate<D>) {
            check(Delegate.IsBound())
            return Delegate.Execute(Forward<A>(Args)...);
        } else if constexpr (MulticastDelegate<D>) {
            Delegate.Broadcast(Forward<A>(Args)...);
        }
    }

} // namespace UE::Ranges

/**
 * Declares a multicast delegate member for a class adding some basic boilerplate methods that are used to bind the
 * delegate and release the binding, but allowing the invocation to remain private.
 * @param DelegateType The type of delegate
 * @param MemberName The name of the member variable
 */
#define UE_MULTICAST_DELEGATE_MEMBER(DelegateType, MemberName) \
    private: \
        DelegateType MemberName; \
    public: \
        FDelegateHandle BindTo##MemberName(DelegateType::FDelegate&& Binding) { \
            return MemberName.Add(MoveTemp(Binding)); \
        } \
        template <typename T, typename F, typename... A> \
            requires std::is_base_of_v<UObject, T> && UE::Ranges::FunctionalType<F> \
        FDelegateHandle BindTo##MemberName(T* Object, F&& Functor, A&&... Args) { \
            if constexpr (StdExt::IsMemberFunction_v<F>) { \
                return MemberName.AddUObject(Object, Forward<F>(Functor), Forward<A>(Args)...);    \
            } else { \
                return MemberName.AddWeakLambda(Object, Forward<F>(Functor), Forward<A>(Args)...); \
            } \
        } \
        void RemoveFrom##MemberName(FDelegateHandle Handle) { \
            MemberName.Remove(Handle); \
        }