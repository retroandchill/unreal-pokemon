// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "UIActionBindingExtensions.generated.h"

class UWidget;
struct FUIActionBindingHandle;

/**
 *
 */
UCLASS(meta = (InternalType))
class UNREALSHARPCOMMONUI_API UUIActionBindingExtensions : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

  public:
    UFUNCTION(meta = (ScriptMethod, ExtensionMethod))
    static bool IsValid(FUIActionBindingHandle Handle);

    UFUNCTION(meta = (ScriptMethod))
    static void Unregister(FUIActionBindingHandle Handle);

    UFUNCTION(meta = (ScriptMethod))
    static void ResetHold(FUIActionBindingHandle Handle);

    UFUNCTION(meta = (ScriptMethod, ExtensionMethod))
    static FName GetActionName(FUIActionBindingHandle Handle);

    UFUNCTION(meta = (ScriptMethod, ExtensionMethod))
    static FText GetDisplayName(FUIActionBindingHandle Handle);

    UFUNCTION(meta = (ScriptMethod))
    static void SetDisplayName(FUIActionBindingHandle Handle, const FText &DisplayName);

    UFUNCTION(meta = (ScriptMethod, ExtensionMethod))
    static bool GetDisplayInActionBar(FUIActionBindingHandle Handle);

    UFUNCTION(meta = (ScriptMethod))
    static void SetDisplayInActionBar(FUIActionBindingHandle Handle, const bool bDisplayInActionBar);

    UFUNCTION(meta = (ScriptMethod, ExtensionMethod))
    static const UWidget *GetBoundWidget(FUIActionBindingHandle Handle);

    UFUNCTION(meta = (ScriptMethod, ExtensionMethod))
    static ULocalPlayer *GetBoundLocalPlayer(FUIActionBindingHandle Handle);
};
