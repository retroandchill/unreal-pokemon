#include "RetroLib/Async/AsyncHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "PlatformFeatures.h"
#include "RetroLib/Functional/Delegates.h"
#include "SaveGameSystem.h"

namespace Retro
{

    UE5Coro::TCoroutine<bool> SaveGameToSlotAsync(USaveGame *SaveGameObject, const FString &SlotName,
                                                  const int32 UserIndex)
    {
        TPromise<bool> SuccessState;
        auto FutureSuccess = SuccessState.GetFuture();
        UGameplayStatics::AsyncSaveGameToSlot(
            SaveGameObject, SlotName, UserIndex,
            Delegates::Create<FAsyncSaveGameToSlotDelegate>(
                [&SuccessState](const FString &, int32, const bool bSuccess) { SuccessState.SetValue(bSuccess); }));
        co_return co_await std::move(FutureSuccess);
    }
} // namespace Retro