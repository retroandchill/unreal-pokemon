// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#if RETROLIB_WITH_UE5CORO
#include "RetroLib/Functional/Delegates.h"
#include "UE5Coro.h"

class USaveGame;

namespace Retro {
    /**
     * Saves the provided save game object to the specified slot asynchronously.
     *
     * @param SaveGameObject A pointer to the save game object to be saved.
     * @param SlotName The name of the slot where the save game will be saved.
     * @param UserIndex The user index to identify the specific user profile.
     * @return A coroutine that resolves to true if the save operation was successful, false otherwise.
     */
    RETROLIB_API UE5Coro::TCoroutine<bool> SaveGameToSlotAsync(USaveGame *SaveGameObject, const FString &SlotName,
                                                               const int32 UserIndex);

    /**
     * Binds a delegate to execute asynchronously and dispatches a specified functor.
     * Waits for the delegate to complete its operation before resuming execution.
     *
     * @param Delegate The delegate that will be bound to handle asynchronous operations.
     * @param Functor The callable object (e.g., lambda, function) to be invoked as part of this coroutine.
     * @return A coroutine that resumes once the delegate completes its execution.
     */
    template <Delegates::NativeDelegate D, typename F>
        requires std::invocable<F> && (std::tuple_size_v<Delegates::TDelegateTuple<D>> == 0)
    UE5Coro::TCoroutine<> BindToDelegateDispatch(D &Delegate, F &&Functor) {
        auto State = MakeShared<TFutureState<int32>>();
        Delegates::TScopedBinding Binding(Delegate, [&State](auto &&...) { State->EmplaceResult(0); });
        std::invoke(std::forward<F>(Functor));
        co_await TFuture<void>(State);
    }

    /**
     * Binds a delegate to execute asynchronously and invokes a specified functor.
     * Waits for the delegate to complete its operation before resuming execution.
     *
     * @param Delegate The delegate to be bound for handling the asynchronous operation.
     * @param Functor The callable object (e.g., lambda or function) to be executed as part of this coroutine.
     * @return A coroutine that resolves to the result of the delegate's execution.
     */
    template <Delegates::NativeDelegate D, typename F>
        requires std::invocable<F> && (std::tuple_size_v<Delegates::TDelegateTuple<D>> > 0)
    UE5Coro::TCoroutine<Delegates::TDelegateTuple<D>> BindToDelegateDispatch(D &Delegate, F &&Functor) {
        auto State = MakeShared<TFutureState<Delegates::TDelegateTuple<D>>>();
        Delegates::TScopedBinding Binding(
            Delegate, [&State]<typename... A>(A &&...Args) { State->EmplaceResult(std::forward<A>(Args)...); });
        std::invoke(std::forward<F>(Functor));
        co_return co_await TFuture<Delegates::TDelegateTuple<D>>(State);
    }
} // namespace Retro
#endif