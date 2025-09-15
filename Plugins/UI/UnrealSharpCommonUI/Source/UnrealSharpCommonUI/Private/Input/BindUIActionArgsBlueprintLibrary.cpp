// "Unreal Pokémon" created by Retro & Chill.

#include "Input/BindUIActionArgsBlueprintLibrary.h"
#include "Input/CSBindUIActionArgs.h"

bool UBindUIActionArgsBlueprintLibrary::IsValid(const FCSBindUIActionArgs &Args)
{
    return Args.IsValid();
}

void UBindUIActionArgsBlueprintLibrary::InitActionTag(FCSBindUIActionArgs &Arg, FUIActionTag InActionTag,
                                                      FOnExecuteActionDelegate InOnExecuteAction)
{
    Arg = MakeShared<FBindUIActionArgs>(
        InActionTag,
        FSimpleDelegate::CreateUFunction(InOnExecuteAction.GetUObject(), InOnExecuteAction.GetFunctionName()));
}

void UBindUIActionArgsBlueprintLibrary::InitActionTagBool(FCSBindUIActionArgs &Arg, FUIActionTag InActionTag,
                                                          bool bShouldDisplayInActionBar,
                                                          FOnExecuteActionDelegate InOnExecuteAction)
{
    Arg = MakeShared<FBindUIActionArgs>(
        InActionTag, bShouldDisplayInActionBar,
        FSimpleDelegate::CreateUFunction(InOnExecuteAction.GetUObject(), InOnExecuteAction.GetFunctionName()));
}

void UBindUIActionArgsBlueprintLibrary::InitDataTableRowHandle(FCSBindUIActionArgs &Arg,
                                                               const FDataTableRowHandle &InActionTableRow,
                                                               FOnExecuteActionDelegate InOnExecuteAction)
{
    Arg = MakeShared<FBindUIActionArgs>(
        InActionTableRow,
        FSimpleDelegate::CreateUFunction(InOnExecuteAction.GetUObject(), InOnExecuteAction.GetFunctionName()));
}

void UBindUIActionArgsBlueprintLibrary::InitDataTableRowHandleBool(FCSBindUIActionArgs &Arg,
                                                                   const FDataTableRowHandle &InActionTableRow,
                                                                   bool bShouldDisplayInActionBar,
                                                                   FOnExecuteActionDelegate InOnExecuteAction)
{
    Arg = MakeShared<FBindUIActionArgs>(
        InActionTableRow, bShouldDisplayInActionBar,
        FSimpleDelegate::CreateUFunction(InOnExecuteAction.GetUObject(), InOnExecuteAction.GetFunctionName()));
}

void UBindUIActionArgsBlueprintLibrary::InitInputAction(FCSBindUIActionArgs &Arg, const UInputAction *InInputAction,
                                                        FOnExecuteActionDelegate InOnExecuteAction)
{
    Arg = MakeShared<FBindUIActionArgs>(
        InInputAction,
        FSimpleDelegate::CreateUFunction(InOnExecuteAction.GetUObject(), InOnExecuteAction.GetFunctionName()));
}

void UBindUIActionArgsBlueprintLibrary::InitInputActionBool(FCSBindUIActionArgs &Arg, const UInputAction *InInputAction,
                                                            bool bShouldDisplayInActionBar,
                                                            FOnExecuteActionDelegate InOnExecuteAction)
{
    Arg = MakeShared<FBindUIActionArgs>(
        InInputAction, bShouldDisplayInActionBar,
        FSimpleDelegate::CreateUFunction(InOnExecuteAction.GetUObject(), InOnExecuteAction.GetFunctionName()));
}

FCSBindUIActionArgs UBindUIActionArgsBlueprintLibrary::Clone(const FCSBindUIActionArgs &Arg)
{
    return Arg.IsValid() ? MakeShared<FBindUIActionArgs>(*Arg.GetArgs()) : Arg;
}

FName UBindUIActionArgsBlueprintLibrary::GetActionName(const FCSBindUIActionArgs &Arg)
{
    return Arg.GetArgs()->GetActionName();
}

bool UBindUIActionArgsBlueprintLibrary::ActionHasHoldMappings(const FCSBindUIActionArgs &Arg)
{
    return Arg.GetArgs()->ActionHasHoldMappings();
}

FUIActionTag UBindUIActionArgsBlueprintLibrary::GetActionTag(const FCSBindUIActionArgs &Arg)
{
    return Arg.GetArgs()->ActionTag;
}

void UBindUIActionArgsBlueprintLibrary::SetActionTag(const FCSBindUIActionArgs &Arg, const FUIActionTag InActionTag)
{
    Arg.GetArgs()->ActionTag = InActionTag;
}

const FDataTableRowHandle &UBindUIActionArgsBlueprintLibrary::GetActionTableRow(const FCSBindUIActionArgs &Arg)
{
    return Arg.GetArgs()->LegacyActionTableRow;
}

void UBindUIActionArgsBlueprintLibrary::SetActionTableRow(const FCSBindUIActionArgs &Arg,
                                                          const FDataTableRowHandle &InActionTableRow)
{
    Arg.GetArgs()->LegacyActionTableRow = InActionTableRow;
}

const UInputAction *UBindUIActionArgsBlueprintLibrary::GetInputAction(const FCSBindUIActionArgs &Arg)
{
    return Arg.GetArgs()->InputAction.Get();
}

void UBindUIActionArgsBlueprintLibrary::SetInputAction(const FCSBindUIActionArgs &Arg,
                                                       const UInputAction *InInputAction)
{
    Arg.GetArgs()->InputAction = InInputAction;
}

ECommonInputMode UBindUIActionArgsBlueprintLibrary::GetInputMode(const FCSBindUIActionArgs &Arg)
{
    return Arg.GetArgs()->InputMode;
}

void UBindUIActionArgsBlueprintLibrary::SetInputMode(const FCSBindUIActionArgs &Arg, ECommonInputMode InInputMode)
{
    Arg.GetArgs()->InputMode = InInputMode;
}

EInputEvent UBindUIActionArgsBlueprintLibrary::GetKeyEvent(const FCSBindUIActionArgs &Arg)
{
    return Arg.GetArgs()->KeyEvent;
}

void UBindUIActionArgsBlueprintLibrary::SetKeyEvent(const FCSBindUIActionArgs &Arg, EInputEvent InKeyEvent)
{
    Arg.GetArgs()->KeyEvent = InKeyEvent;
}

const TSet<ECommonInputType> &UBindUIActionArgsBlueprintLibrary::GetInputTypesExemptFromValidKeyCheck(
    const FCSBindUIActionArgs &Arg)
{
    return Arg.GetArgs()->InputTypesExemptFromValidKeyCheck;
}

void UBindUIActionArgsBlueprintLibrary::SetInputTypesExemptFromValidKeyCheck(
    const FCSBindUIActionArgs &Arg, TSet<ECommonInputType> InInputTypesExemptFromValidKeyCheck)
{
    Arg.GetArgs()->InputTypesExemptFromValidKeyCheck = MoveTemp(InInputTypesExemptFromValidKeyCheck);
}

bool UBindUIActionArgsBlueprintLibrary::GetIsPersistent(const FCSBindUIActionArgs &Arg)
{
    return Arg.GetArgs()->bIsPersistent;
}

void UBindUIActionArgsBlueprintLibrary::SetIsPersistent(const FCSBindUIActionArgs &Arg, const bool bInIsPersistent)
{

    Arg.GetArgs()->bIsPersistent = bInIsPersistent;
}

bool UBindUIActionArgsBlueprintLibrary::GetConsumeInput(const FCSBindUIActionArgs &Arg)
{
    return Arg.GetArgs()->bConsumeInput;
}

void UBindUIActionArgsBlueprintLibrary::SetConsumeInput(const FCSBindUIActionArgs &Arg, const bool bInConsumeInput)
{
    Arg.GetArgs()->bConsumeInput = bInConsumeInput;
}

bool UBindUIActionArgsBlueprintLibrary::GetDisplayInActionBar(const FCSBindUIActionArgs &Arg)
{
    return Arg.GetArgs()->bDisplayInActionBar;
}

void UBindUIActionArgsBlueprintLibrary::SetDisplayInActionBar(const FCSBindUIActionArgs &Arg,
                                                              const bool bInDisplayInActionBar)
{
    Arg.GetArgs()->bDisplayInActionBar = bInDisplayInActionBar;
}

bool UBindUIActionArgsBlueprintLibrary::GetForceHold(const FCSBindUIActionArgs &Arg)
{
    return Arg.GetArgs()->bForceHold;
}

void UBindUIActionArgsBlueprintLibrary::SetForceHold(const FCSBindUIActionArgs &Arg, const bool bInForceHold)
{
    Arg.GetArgs()->bForceHold = bInForceHold;
}

const FText &UBindUIActionArgsBlueprintLibrary::GetOverrideDisplayName(const FCSBindUIActionArgs &Arg)
{
    return Arg.GetArgs()->OverrideDisplayName;
}

void UBindUIActionArgsBlueprintLibrary::SetOverrideDisplayName(const FCSBindUIActionArgs &Arg,
                                                               FText InOverrideDisplayName)
{
    Arg.GetArgs()->OverrideDisplayName = MoveTemp(InOverrideDisplayName);
}

int32 UBindUIActionArgsBlueprintLibrary::GetPriorityWithinCollection(const FCSBindUIActionArgs &Arg)
{
    return Arg.GetArgs()->PriorityWithinCollection;
}

void UBindUIActionArgsBlueprintLibrary::SetPriorityWithinCollection(const FCSBindUIActionArgs &Arg,
                                                                    const int32 InPriorityWithinCollection)
{
    Arg.GetArgs()->PriorityWithinCollection = InPriorityWithinCollection;
}

void UBindUIActionArgsBlueprintLibrary::SetOnExecuteAction(const FCSBindUIActionArgs &Arg,
                                                           FOnExecuteActionDelegate InOnExecuteAction)
{
    Arg.GetArgs()->OnExecuteAction.BindUFunction(InOnExecuteAction.GetUObject(), InOnExecuteAction.GetFunctionName());
}

void UBindUIActionArgsBlueprintLibrary::SetOnHoldActionProgressed(
    const FCSBindUIActionArgs &Arg, FOnHoldActionProgressedDelegate InOnHoldActionProgressed)
{
    Arg.GetArgs()->OnHoldActionProgressed.BindUFunction(InOnHoldActionProgressed.GetUObject(),
                                                        InOnHoldActionProgressed.GetFunctionName());
}

void UBindUIActionArgsBlueprintLibrary::SetOnHoldActionPressed(const FCSBindUIActionArgs &Arg,
                                                               FOnHoldActionPressedDelegate InOnHoldActionPressed)
{
    Arg.GetArgs()->OnHoldActionPressed.BindUFunction(InOnHoldActionPressed.GetUObject(),
                                                     InOnHoldActionPressed.GetFunctionName());
}

void UBindUIActionArgsBlueprintLibrary::SetOnHoldActionReleased(const FCSBindUIActionArgs &Arg,
                                                                FOnHoldActionReleasedDelegate InOnHoldActionReleased)
{
    Arg.GetArgs()->OnHoldActionReleased.BindUFunction(InOnHoldActionReleased.GetUObject(),
                                                      InOnHoldActionReleased.GetFunctionName());
}