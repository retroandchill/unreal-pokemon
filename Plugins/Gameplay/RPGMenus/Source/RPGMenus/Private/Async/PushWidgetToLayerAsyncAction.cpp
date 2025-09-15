// Fill out your copyright notice in the Description page of Project Settings.


#include "Async/PushWidgetToLayerAsyncAction.h"

#include "PrimaryGameLayout.h"

UPushWidgetToLayerAsyncAction *UPushWidgetToLayerAsyncAction::PushWidgetToLayerAsync(APlayerController* OwningPlayer,
                                                                                     TSoftClassPtr<UCommonActivatableWidget> InWidgetClass, const FGameplayTag InLayerName, const bool bSuspendInputUntilComplete)
{
    if (InWidgetClass.IsNull())
    {
        FFrame::KismetExecutionMessage(TEXT("PushContentToLayerForPlayer was passed a null WidgetClass"), ELogVerbosity::Error);
        return nullptr;
    }

    const auto World = GEngine->GetWorldFromContextObject(OwningPlayer, EGetWorldErrorMode::LogAndReturnNull);
    if (World == nullptr)
    {
        return nullptr;
    }

    const auto Action = NewObject<UPushWidgetToLayerAsyncAction>();
    Action->WidgetClass = MoveTemp(InWidgetClass);
    Action->OwningPlayerPtr = OwningPlayer;
    Action->LayerName = InLayerName;
    Action->bSuspendInputUntilComplete = bSuspendInputUntilComplete;
    Action->RegisterWithGameInstance(World);

    return Action;
}

void UPushWidgetToLayerAsyncAction::Activate()
{
    const auto RootLayout = UPrimaryGameLayout::GetInstance(OwningPlayerPtr.Get());
    if (RootLayout == nullptr)
    {
        SetReadyToDestroy();
        return;
    }
    
    TWeakObjectPtr WeakThis = this;
    StreamingHandle = RootLayout->PushWidgetToLayerStackAsync<UCommonActivatableWidget>(LayerName, bSuspendInputUntilComplete, MoveTemp(WidgetClass),
        [this, WeakThis](const EAsyncWidgetLayerState State, UCommonActivatableWidget* Widget)
        {
            if (WeakThis.IsValid())
            {
                switch (State)
                {
                case EAsyncWidgetLayerState::Initialize:
                    ConfigureWidget.Broadcast(Widget);
                    break;
                case EAsyncWidgetLayerState::AfterPush:
                    OnComplete.Broadcast(Widget);
                    SetReadyToDestroy();
                    break;
                case EAsyncWidgetLayerState::Canceled:
                    SetReadyToDestroy();
                    break;
            }
        }
        SetReadyToDestroy();
    });
}

void UPushWidgetToLayerAsyncAction::Cancel()
{
    Super::Cancel();

    if (StreamingHandle.IsValid())
    {
        StreamingHandle->CancelHandle();
        StreamingHandle.Reset();
    }
}
