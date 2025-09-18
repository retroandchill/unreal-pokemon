// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StreamableManager.h"
#include "GameplayTagContainer.h"
#include "UnrealSharpAsync/Public/CSAsyncActionBase.h"

#include "CSPushWidgetToLayerAsync.generated.h"

class UCommonActivatableWidget;

UENUM()
enum class EAsyncLoadSuccessState : uint8
{
    InProgress,
    NoSuchLayer,
    Cancelled,
    BeforePush,
    Success
};

/**
 *
 */
UCLASS(meta = (InternalType))
class INTERACTIVEUI_API UCSPushWidgetToLayerAsync : public UCSAsyncActionBase
{
    GENERATED_BODY()

  public:
    UFUNCTION(meta = (ScriptMethod))
    void PushWidgetToLayerStack(const APlayerController *PlayerController, FGameplayTag InLayerName,
                                const bool bSuspendInputUntilComplete,
                                TSoftClassPtr<UCommonActivatableWidget> ActivatableWidgetClass);

    UFUNCTION(meta = (DeterminesOutputType = WidgetClass, DynamicOutputParam = OutWidget, ScriptMethod))
    EAsyncLoadSuccessState GetResult(UCommonActivatableWidget *&OutWidget) const;

    UFUNCTION(meta = (ScriptMethod))
    void Cancel();

  protected:
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
