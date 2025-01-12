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
        requires std::invocable<F>
    UE5Coro::TCoroutine<> BindToDelegateDispatch(D& Delegate, F&& Functor) {
        auto State = MakeShared<TFutureState<int32>>();
        if constexpr (Delegates::UnicastDelegate<D>) {
            Delegate.BindLambda([&State](auto&&...) {
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
}
#endif