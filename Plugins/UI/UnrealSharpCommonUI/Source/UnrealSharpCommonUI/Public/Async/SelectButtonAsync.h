// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CSAsyncActionBase.h"

#include "SelectButtonAsync.generated.h"

class UCommonButtonGroupBase;
class UCommonButtonBase;

/**
 * @class USelectButtonAsync
 * @brief An asynchronous action class for managing button selection within a button group.
 *
 * This class allows the selection of a button from a specified button group and notifies the caller
 * when the selection is made or cancelled. It extends the functionality of UCSAsyncActionBase for
 * handling asynchronous operations involving UI elements in Unreal Engine.
 */
UCLASS(meta = (InternalType))
class UNREALSHARPCOMMONUI_API USelectButtonAsync : public UCSAsyncActionBase
{
    GENERATED_BODY()

  public:
    /**
     * @brief Selects a button group to monitor and binds the necessary event for button click selection.
     *
     * This method associates a button group with the asynchronous selection process. It listens for
     * button click events within the specified group and triggers the appropriate action when a button
     * is selected.
     *
     * @param InButtonGroup A pointer to the button group that will be monitored for button selections.
     */
    UFUNCTION(meta = (ScriptMethod))
    void SelectButton(UCommonButtonGroupBase *InButtonGroup);

  private:
    void OnSelectButton(UCommonButtonBase *Button, int32 Index);

    UFUNCTION(meta = (ScriptMethod))
    void CancelSelection();

    UPROPERTY()
    TObjectPtr<UCommonButtonGroupBase> ButtonGroup;

    UPROPERTY()
    TObjectPtr<UCommonButtonBase> SelectedButton;

    UPROPERTY()
    int32 SelectedButtonIndex;

    UPROPERTY()
    bool bCancelled = false;

    FDelegateHandle DelegateHandle;
};
