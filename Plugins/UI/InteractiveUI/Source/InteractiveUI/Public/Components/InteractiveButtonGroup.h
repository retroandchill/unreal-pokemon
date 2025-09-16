// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Groups/CommonButtonGroupBase.h"
#include "InteractiveButtonGroup.generated.h"

class UCommonActivatableWidget;

DECLARE_MULTICAST_DELEGATE_TwoParams(FNativeAddButton, int32, UCommonButtonBase*);
DECLARE_MULTICAST_DELEGATE_OneParam(FNativeRemoveButton, UCommonButtonBase*);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAddButton, int32, Index, UCommonButtonBase*, Button);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRemoveButton, UCommonButtonBase*, Button);

/**
 * 
 */
UCLASS()
class INTERACTIVEUI_API UInteractiveButtonGroup : public UCommonButtonGroupBase
{
    GENERATED_BODY()

public:
    FDelegateHandle BindToPlaceButton(FNativeAddButton::FDelegate Delegate)
    {
        return NativeOnAddButton.Add(MoveTemp(Delegate));   
    }

    void UnbindFromPlaceButton(const FDelegateHandle Handle)
    {
        NativeOnAddButton.Remove(Handle);
    }

    FDelegateHandle BindToRemoveButton(FNativeRemoveButton::FDelegate Delegate)
    {
        return NativeOnRemoveButton.Add(MoveTemp(Delegate));   
    }

    void UnbindFromRemoveButton(const FDelegateHandle Handle)
    {
        NativeOnRemoveButton.Remove(Handle);
    }
    
protected:
    void OnWidgetAdded(UWidget* Widget) override;
    void OnWidgetRemoved(UWidget* Widget) override;

private:
    FNativeAddButton NativeOnAddButton;
    FNativeRemoveButton NativeOnRemoveButton;

    UPROPERTY(BlueprintAssignable, Category = "Events", meta = (ScriptName = "OnAddButtonDelegate"))
    FAddButton OnAddButton;

    UPROPERTY(BlueprintAssignable, Category = "Events", meta = (ScriptName = "OnRemoveButtonDelegate"))
    FRemoveButton OnRemoveButton;
    
};
