// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Ranges/Concepts/Delegates.h"

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
        void RemoveFrom##MemberName(FDelegateHandle Handle) { \
            MemberName.Remove(Handle); \
        }