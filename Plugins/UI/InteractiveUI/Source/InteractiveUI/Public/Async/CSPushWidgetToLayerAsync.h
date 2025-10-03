// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StreamableManager.h"
#include "GameplayTagContainer.h"
#include "UnrealSharpAsync/Public/CSAsyncActionBase.h"

#include "CSPushWidgetToLayerAsync.generated.h"

class UCommonActivatableWidget;

/**
 * Enum representing the state of an asynchronous widget loading and pushing operation.
 */
UENUM()
enum class EAsyncLoadSuccessState : uint8
{
    /**
     * The operation is currently ongoing.
     */
    InProgress,

    /**
     * The specified widget layer does not exist.
     */
    NoSuchLayer,

    /**
     * The operation was cancelled before completion.
     */
    Cancelled,

    /**
     * The operation reached the stage preceding the widget being pushed to a layer.
     */
    BeforePush,

    /**
     * The operation completed successfully and the widget was pushed to the layer.
     */
    Success
};

/**
 * Class representing an asynchronous operation to push a widget to a specified layer.
 * This operation allows managing the visibility and composition of UI layers by asynchronously
 * loading and adding widgets to the desired layer stack.
 */
UCLASS(meta = (InternalType))
class INTERACTIVEUI_API UCSPushWidgetToLayerAsync : public UCSAsyncActionBase
{
    GENERATED_BODY()

  public:
    /**
     * Asynchronously pushes a widget to a specified layer stack, optionally suspending input until the operation
     * completes. The function handles the lifecycle of the widget during the loading process and invokes appropriate
     * states during the operation.
     *
     * @param PlayerController A pointer to the player controller owning the operation. It identifies which player's UI
     * stack the widget should be added to.
     * @param InLayerName The name of the layer to which the widget should be pushed.
     * @param bSuspendInputUntilComplete A flag indicating whether input should be suspended until the widget is
     * successfully loaded and pushed.
     * @param ActivatableWidgetClass A soft reference to the activatable widget class to be instantiated and pushed to
     * the layer stack.
     */
    UFUNCTION(meta = (ScriptMethod))
    void PushWidgetToLayerStack(const APlayerController *PlayerController, FGameplayTag InLayerName,
                                const bool bSuspendInputUntilComplete,
                                TSoftClassPtr<UCommonActivatableWidget> ActivatableWidgetClass);

    /**
     * Retrieves the result of the asynchronous widget loading and pushing operation.
     * If the operation was successful, the loaded widget is returned via the OutWidget parameter.
     *
     * @param OutWidget A reference to a pointer that will be set to the successfully loaded widget if the operation
     * succeeded, or null if it failed.
     * @return The current state of the asynchronous operation as an EAsyncLoadSuccessState enum value.
     */
    UFUNCTION(meta = (DeterminesOutputType = WidgetClass, DynamicOutputParam = OutWidget, ScriptMethod))
    EAsyncLoadSuccessState GetResult(UCommonActivatableWidget *&OutWidget) const;

    /**
     * Cancels the ongoing asynchronous operation of pushing a widget to a specified layer.
     *
     * This method terminates the operation and transitions its state to `EAsyncLoadSuccessState::Cancelled`.
     * If a streaming handle is active, the cancellation process ensures that no further loading or pushing actions
     * occur.
     */
    UFUNCTION(meta = (ScriptMethod))
    void Cancel();

  protected:
    /**
     * Callback method invoked upon completion of the asynchronous widget loading and pushing operation.
     *
     * @param InState The final state of the asynchronous operation, represented by the EAsyncLoadSuccessState enum.
     * @param InWidget A pointer to the widget that was loaded and pushed to the layer, or nullptr if the operation did
     * not succeed.
     * @param bDispose A boolean flag indicating whether the widget should be disposed of after the operation concludes.
     */
    void OnAsyncLoadComplete(EAsyncLoadSuccessState InState, UCommonActivatableWidget *InWidget = nullptr,
                             bool bDispose = true);

  private:
    UPROPERTY()
    FGameplayTag LayerName;

    UPROPERTY()
    TObjectPtr<UCommonActivatableWidget> Widget;

    EAsyncLoadSuccessState State = EAsyncLoadSuccessState::InProgress;
    TSharedPtr<FStreamableHandle> StreamingHandle;
};
