// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#if RETROLIB_WITH_UE5CORO
#include "UE5Coro.h"
#include "RetroLib/Functional/Delegates.h"

class USaveGame;

namespace Retro {
    RETROLIB_API UE5Coro::TCoroutine<bool> SaveGameToSlotAsync(USaveGame *SaveGameObject, const FString &SlotName,
                                                               const int32 UserIndex);

    template <Delegates::NativeDelegate D,  typename F>
        requires std::invocable<F> && (std::tuple_size_v<Delegates::TDelegateTuple<D>> == 0)
    UE5Coro::TCoroutine<> BindToDelegateDispatch(D& Delegate, F&& Functor) {
        auto State = MakeShared<TFutureState<int32>>();
        if constexpr (Delegates::UnicastDelegate<D>) {
            Delegate.BindLamba([&State](auto&&...) {
                State->EmplaceResult(0);
            });
        } else {
            Delegate.AddLambda([&State](auto&&...) {
                State->EmplaceResult(0);
            });
        }
        
        std::invoke(std::forward<F>(Functor));
        co_await TFuture<void>(State);
    }

    template <Delegates::NativeDelegate D,  typename F>
        requires std::invocable<F> && (std::tuple_size_v<Delegates::TDelegateTuple<D>> > 0)
    UE5Coro::TCoroutine<Delegates::TDelegateTuple<D>> BindToDelegateDispatch(D& Delegate, F&& Functor) {
        auto State = MakeShared<TFutureState<Delegates::TDelegateTuple<D>>>();
        if constexpr (Delegates::UnicastDelegate<D>) {
            Delegate.BindLamba([&State]<typename... A>(A&&... Args) {
                State->EmplaceResult(std::forward<A>(Args)...);
            });
        } else {
            Delegate.AddLambda([&State]<typename... A>(A&&... Args) {
                State->EmplaceResult(std::forward<A>(Args)...);
            });
        }
        
        std::invoke(std::forward<F>(Functor));
        co_return co_await TFuture<Delegates::TDelegateTuple<D>>(State);
    }
}
#endif