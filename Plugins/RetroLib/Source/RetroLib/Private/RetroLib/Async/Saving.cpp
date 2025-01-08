#include "RetroLib/Async/Saving.h"
#include "Kismet/GameplayStatics.h"
#include "PlatformFeatures.h"
#include "RetroLib/Functional/Delegates.h"
#include "SaveGameSystem.h"

namespace Retro {
    UE5Coro::TCoroutine<bool> SaveGameToSlotAsync(USaveGame *SaveGameObject, const FString &SlotName,
                                                  const int32 UserIndex) {
        auto SuccessState = MakeShared<TFutureState<bool>>();
        TFuture<bool> FutureSuccess(SuccessState);
        UGameplayStatics::AsyncSaveGameToSlot(
            SaveGameObject, SlotName, UserIndex,
            Delegates::Create<FAsyncSaveGameToSlotDelegate>(
                [SuccessState](const FString &, int32, bool bSuccess) { SuccessState->EmplaceResult(bSuccess); }));
        co_return co_await std::move(FutureSuccess);
    }
} // namespace Retro