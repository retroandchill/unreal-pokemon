// Fill out your copyright notice in the Description page of Project Settings.


#include "Async/CSPushWidgetToLayerAsync.h"

#include "PrimaryGameLayout.h"

void UCSPushWidgetToLayerAsync::PushWidgetToLayerStack(const APlayerController* PlayerController,
                                                       const FGameplayTag InLayerName,
                                                       const bool bSuspendInputUntilComplete,
                                                       TSoftClassPtr<UCommonActivatableWidget> ActivatableWidgetClass)
{
    LayerName = InLayerName;
    const auto RootLayout = UPrimaryGameLayout::GetInstance(PlayerController);
    if (RootLayout == nullptr)
    {
        OnAsyncLoadComplete(EAsyncLoadSuccessState::NoSuchLayer);
        return;
    }

    OnAsyncLoadComplete(EAsyncLoadSuccessState::InProgress);
    TWeakObjectPtr WeakThis = this;
    StreamingHandle = RootLayout->PushWidgetToLayerStackAsync(InLayerName, bSuspendInputUntilComplete, MoveTemp(ActivatableWidgetClass),
        [this, WeakThis](const EAsyncWidgetLayerState State, UCommonActivatableWidget* Widget)
        {
            if (!WeakThis.IsValid())
            {
                return;
            }

            switch (State)
            {
            case EAsyncWidgetLayerState::Initialize:
                OnAsyncLoadComplete(EAsyncLoadSuccessState::BeforePush, Widget, false);
                break;
            case EAsyncWidgetLayerState::Canceled:
                OnAsyncLoadComplete(EAsyncLoadSuccessState::Cancelled);
                break;
            case EAsyncWidgetLayerState::AfterPush:
                OnAsyncLoadComplete(EAsyncLoadSuccessState::Success, Widget);
                break;
            }
        });
}

EAsyncLoadSuccessState UCSPushWidgetToLayerAsync::GetResult(UCommonActivatableWidget*& OutWidget) const
{
    if (State == EAsyncLoadSuccessState::Success)
    {
        OutWidget = Widget;
        return EAsyncLoadSuccessState::Success;
    }

    OutWidget = nullptr;
    return State;
}

void UCSPushWidgetToLayerAsync::Cancel()
{
    StreamingHandle->CancelHandle();
    OnAsyncLoadComplete(EAsyncLoadSuccessState::Cancelled);
}

void UCSPushWidgetToLayerAsync::OnAsyncLoadComplete(const EAsyncLoadSuccessState InState, UCommonActivatableWidget* InWidget, const bool bDispose)
{
    State = InState;
    Widget = InWidget;
    InvokeManagedCallback(bDispose);
}
