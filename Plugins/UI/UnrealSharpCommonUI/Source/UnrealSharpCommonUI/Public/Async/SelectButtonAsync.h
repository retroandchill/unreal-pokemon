// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CSAsyncActionBase.h"

#include "SelectButtonAsync.generated.h"

class UCommonButtonGroupBase;
class UCommonButtonBase;
/**
 *
 */
UCLASS(meta = (InternalType))
class UNREALSHARPCOMMONUI_API USelectButtonAsync : public UCSAsyncActionBase
{
    GENERATED_BODY()

  public:
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
