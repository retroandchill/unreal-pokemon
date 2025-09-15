// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "CommonInputModeTypes.h"
#include "CommonInputTypeEnum.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UITag.h"

#include "BindUIActionArgsBlueprintLibrary.generated.h"

class UInputAction;
struct FCSBindUIActionArgs;

DECLARE_DYNAMIC_DELEGATE(FOnExecuteActionDelegate);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnHoldActionProgressedDelegate, float, Progress);
DECLARE_DYNAMIC_DELEGATE(FOnHoldActionPressedDelegate);
DECLARE_DYNAMIC_DELEGATE(FOnHoldActionReleasedDelegate);

/**
 *
 */
UCLASS(meta = (InternalType))
class UNREALSHARPCOMMONUI_API UBindUIActionArgsBlueprintLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

  public:
    UFUNCTION(meta = (ScriptMethod))
    static bool IsValid(const FCSBindUIActionArgs &Args);

    UFUNCTION(meta = (ScriptMethod))
    static void InitActionTag(UPARAM(ref) FCSBindUIActionArgs &Arg, FUIActionTag InActionTag,
                              FOnExecuteActionDelegate InOnExecuteAction);

    UFUNCTION(meta = (ScriptMethod))
    static void InitActionTagBool(UPARAM(ref) FCSBindUIActionArgs &Arg, FUIActionTag InActionTag,
                                  bool bShouldDisplayInActionBar, FOnExecuteActionDelegate InOnExecuteAction);

    UFUNCTION(meta = (ScriptMethod))
    static void InitDataTableRowHandle(UPARAM(ref) FCSBindUIActionArgs &Arg,
                                       const FDataTableRowHandle &InActionTableRow,
                                       FOnExecuteActionDelegate InOnExecuteAction);

    UFUNCTION(meta = (ScriptMethod))
    static void InitDataTableRowHandleBool(UPARAM(ref) FCSBindUIActionArgs &Arg,
                                           const FDataTableRowHandle &InActionTableRow, bool bShouldDisplayInActionBar,
                                           FOnExecuteActionDelegate InOnExecuteAction);

    UFUNCTION(meta = (ScriptMethod))
    static void InitInputAction(UPARAM(ref) FCSBindUIActionArgs &Arg, const UInputAction *InInputAction,
                                FOnExecuteActionDelegate InOnExecuteAction);

    UFUNCTION(meta = (ScriptMethod))
    static void InitInputActionBool(UPARAM(ref) FCSBindUIActionArgs &Arg, const UInputAction *InInputAction,
                                    bool bShouldDisplayInActionBar, FOnExecuteActionDelegate InOnExecuteAction);

    UFUNCTION(meta = (ScriptMethod))
    static FCSBindUIActionArgs Clone(const FCSBindUIActionArgs &Arg);

    UFUNCTION(meta = (ScriptMethod))
    static FName GetActionName(const FCSBindUIActionArgs &Arg);

    UFUNCTION(meta = (ScriptMethod))
    static bool ActionHasHoldMappings(const FCSBindUIActionArgs &Arg);

    UFUNCTION(meta = (ScriptMethod))
    static FUIActionTag GetActionTag(const FCSBindUIActionArgs &Arg);

    UFUNCTION(meta = (ScriptMethod))
    static void SetActionTag(const FCSBindUIActionArgs &Arg, FUIActionTag InActionTag);

    UFUNCTION(meta = (ScriptMethod))
    static const FDataTableRowHandle &GetActionTableRow(const FCSBindUIActionArgs &Arg);

    UFUNCTION(meta = (ScriptMethod))
    static void SetActionTableRow(const FCSBindUIActionArgs &Arg, const FDataTableRowHandle &InActionTableRow);

    UFUNCTION(meta = (ScriptMethod))
    static const UInputAction *GetInputAction(const FCSBindUIActionArgs &Arg);

    UFUNCTION(meta = (ScriptMethod))
    static void SetInputAction(const FCSBindUIActionArgs &Arg, const UInputAction *InInputAction);

    UFUNCTION(meta = (ScriptMethod))
    static ECommonInputMode GetInputMode(const FCSBindUIActionArgs &Arg);

    UFUNCTION(meta = (ScriptMethod))
    static void SetInputMode(const FCSBindUIActionArgs &Arg, ECommonInputMode InInputMode);

    UFUNCTION(meta = (ScriptMethod))
    static EInputEvent GetKeyEvent(const FCSBindUIActionArgs &Arg);

    UFUNCTION(meta = (ScriptMethod))
    static void SetKeyEvent(const FCSBindUIActionArgs &Arg, EInputEvent InKeyEvent);

    UFUNCTION(meta = (ScriptMethod))
    static const TSet<ECommonInputType> &GetInputTypesExemptFromValidKeyCheck(const FCSBindUIActionArgs &Arg);

    UFUNCTION(meta = (ScriptMethod))
    static void SetInputTypesExemptFromValidKeyCheck(const FCSBindUIActionArgs &Arg,
                                                     TSet<ECommonInputType> InInputTypesExemptFromValidKeyCheck);

    UFUNCTION(meta = (ScriptMethod))
    static bool GetIsPersistent(const FCSBindUIActionArgs &Arg);

    UFUNCTION(meta = (ScriptMethod))
    static void SetIsPersistent(const FCSBindUIActionArgs &Arg, bool bInIsPersistent);

    UFUNCTION(meta = (ScriptMethod))
    static bool GetConsumeInput(const FCSBindUIActionArgs &Arg);

    UFUNCTION(meta = (ScriptMethod))
    static void SetConsumeInput(const FCSBindUIActionArgs &Arg, bool bInConsumeInput);

    UFUNCTION(meta = (ScriptMethod))
    static bool GetDisplayInActionBar(const FCSBindUIActionArgs &Arg);

    UFUNCTION(meta = (ScriptMethod))
    static void SetDisplayInActionBar(const FCSBindUIActionArgs &Arg, bool bInDisplayInActionBar);

    UFUNCTION(meta = (ScriptMethod))
    static bool GetForceHold(const FCSBindUIActionArgs &Arg);

    UFUNCTION(meta = (ScriptMethod))
    static void SetForceHold(const FCSBindUIActionArgs &Arg, bool bInForceHold);

    UFUNCTION(meta = (ScriptMethod))
    static const FText &GetOverrideDisplayName(const FCSBindUIActionArgs &Arg);

    UFUNCTION(meta = (ScriptMethod))
    static void SetOverrideDisplayName(const FCSBindUIActionArgs &Arg, FText InOverrideDisplayName);

    UFUNCTION(meta = (ScriptMethod))
    static int32 GetPriorityWithinCollection(const FCSBindUIActionArgs &Arg);

    UFUNCTION(meta = (ScriptMethod))
    static void SetPriorityWithinCollection(const FCSBindUIActionArgs &Arg, int32 InPriorityWithinCollection);

    UFUNCTION(meta = (ScriptMethod))
    static void SetOnExecuteAction(const FCSBindUIActionArgs &Arg, FOnExecuteActionDelegate InOnExecuteAction);

    UFUNCTION(meta = (ScriptMethod))
    static void SetOnHoldActionProgressed(const FCSBindUIActionArgs &Arg,
                                          FOnHoldActionProgressedDelegate InOnHoldActionProgressed);

    UFUNCTION(meta = (ScriptMethod))
    static void SetOnHoldActionPressed(const FCSBindUIActionArgs &Arg,
                                       FOnHoldActionPressedDelegate InOnHoldActionPressed);

    UFUNCTION(meta = (ScriptMethod))
    static void SetOnHoldActionReleased(const FCSBindUIActionArgs &Arg,
                                        FOnHoldActionReleasedDelegate InOnHoldActionReleased);
};
