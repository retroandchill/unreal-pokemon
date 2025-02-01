// "Unreal Pokémon" created by Retro & Chill.

#include "Actions/PushContentToLayerForPlayer.h"

UPushContentToLayerForPlayer *
UPushContentToLayerForPlayer::PushContentToLayerForPlayer(APlayerController *OwningPlayer,
                                                          TSoftClassPtr<UCommonActivatableWidget> WidgetClass,
                                                          FGameplayTag LayerName, bool bSuspendInputUntilComplete) {
    if (WidgetClass.IsNull()) {
        FFrame::KismetExecutionMessage(TEXT("PushContentToLayerForPlayer was passed a null WidgetClass"),
                                       ELogVerbosity::Error);
        return nullptr;
    }

    if (UWorld *World = GEngine->GetWorldFromContextObject(OwningPlayer, EGetWorldErrorMode::LogAndReturnNull)) {
        auto Action = NewObject<UPushContentToLayerForPlayer>();
        Action->WidgetClass = WidgetClass;
        Action->OwningPlayerPtr = OwningPlayer;
        Action->LayerName = LayerName;
        Action->bSuspendInputUntilComplete = bSuspendInputUntilComplete;
        Action->RegisterWithGameInstance(World);

        return Action;
    }

    return nullptr;
}

void UPushContentToLayerForPlayer::Activate() {
    [this](UE5Coro::TLatentContext<APlayerController> Context) -> UE5Coro::TCoroutine<> {
        if (auto RootLayer = UPrimaryGameLayout::Get(Context.Target); RootLayer != nullptr) {
            Coroutine.Emplace(PushToLayer(RootLayer));
            auto Result = co_await *Coroutine;
            AfterPush.Broadcast(Result);
        }

        SetReadyToDestroy();
    }(OwningPlayerPtr.Get());
}

void UPushContentToLayerForPlayer::Cancel() {
    Super::Cancel();

    if (Coroutine.IsSet()) {
        Coroutine->Cancel();
        Coroutine.Reset();
    }
}

UE5Coro::TCoroutine<UCommonActivatableWidget *>
UPushContentToLayerForPlayer::PushToLayer(UE5Coro::TLatentContext<UPrimaryGameLayout> Context) {
    UE5Coro::FOnCoroutineCanceled Canceled([this] { SetReadyToDestroy(); });

    co_return co_await Context.Target->PushWidgetToLayerStackAsync(LayerName, std::move(WidgetClass),
                                                                   bSuspendInputUntilComplete);
}