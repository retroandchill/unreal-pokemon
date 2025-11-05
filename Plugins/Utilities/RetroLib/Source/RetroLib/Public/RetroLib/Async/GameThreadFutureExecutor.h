// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#ifdef __UNREAL__
#if !RETROLIB_WITH_MODULES
#include "Tickable.h"
#endif

#include "RetroLib/Functional/Delegates.h"

namespace Retro {
    /**
     * @class TGameThreadFutureExecutor
     *
     * @brief Executes future tasks on the game thread.
     *
     * The TGameThreadFutureExecutor class provides functionality to execute
     * tasks that are scheduled to run in the future on the main game thread.
     * This ensures that the tasks are executed in the correct context and
     * at the appropriate time, avoiding concurrency issues.
     *
     * The class supports scheduling tasks that must run sequentially on the
     * game thread, preserving the order of execution and the integrity of
     * game state.
     */
    template <typename T>
    class TGameThreadFutureExecutor : public FTickableGameObject {
      public:
        using FCompleteDelegate = TMulticastDelegate<void(T)>;

        /**
         * @brief Constructs a TGameThreadFutureExecutor with the provided future.
         *
         * Initializes the executor to manage and execute the provided future task on the game thread.
         *
         * @param Future The future task to be executed on the game thread.
         * @return A TGameThreadFutureExecutor instance configured with the provided future.
         */
        explicit TGameThreadFutureExecutor(TFuture<T> &&Future) : Future(std::move(Future)) {
        }

        /**
         * @brief Adds a task to the OnComplete delegate that will be executed when the task is complete.
         *
         * This method immediately executes the task if the Value is already set. Otherwise, it adds the task
         * to the delegate to be executed upon completion.
         *
         * @param Args The arguments to bind to the task being added to the OnComplete delegate.
         * @return A handle to the delegate that allows for managing the task.
         */
        template <typename... A>
            requires Delegates::CanAddToDelegate<FCompleteDelegate, A...>
        FDelegateHandle AddOnCompleteTask(A &&...Args) {
            if (Value.IsSet()) {
                std::invoke(CreateBinding(std::forward<A>(Args)...), *Value);
            }

            return Delegates::Add(OnComplete, std::forward<A>(Args)...);
        }

        void Tick(float DeltaTime) override {
            if (!Future.IsReady()) {
                return;
            }

            Value.Emplace(Future.Consume());
            OnComplete.Broadcast(*Value);
        }

        TStatId GetStatId() const override {
            RETURN_QUICK_DECLARE_CYCLE_STAT(UTickBasedClock, STATGROUP_Tickables);
        }

      private:
        TFuture<T> Future;
        TOptional<T> Value;
        FCompleteDelegate OnComplete;
    };
} // namespace Retro
#endif