// "Unreal Pokémon" created by Retro & Chill.

#include "RetroLib/Async/BlueprintCoroutineActionBase.h"

#if RETROLIB_WITH_UE5CORO
void UBlueprintCoroutineActionBase::Activate() {
    [](UE5Coro::TLatentContext<UBlueprintCoroutineActionBase> This) -> UE5Coro::TCoroutine<> {
        co_await This->ExecuteCoroutine();
        This->SetReadyToDestroy();
    }(this);
}

UWorld *UBlueprintCoroutineActionBase::GetWorld() const {
    return WorldContext->GetWorld();
}

UE5Coro::TCoroutine<> UBlueprintCoroutineActionBase::UntilComplete(FForceLatentCoroutine Coro) const {
    co_await UE5Coro::Latent::Until([this] { return !HasAnyFlags(RF_StrongRefOnFrame); });
}
#endif