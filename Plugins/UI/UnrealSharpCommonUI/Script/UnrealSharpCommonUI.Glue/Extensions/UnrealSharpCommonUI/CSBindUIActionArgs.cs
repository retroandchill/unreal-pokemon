using UnrealSharp.CommonInput;
using UnrealSharp.CommonUI;
using UnrealSharp.Engine;
using UnrealSharp.EnhancedInput;

namespace UnrealSharp.UnrealSharpCommonUI;

public partial record struct FCSBindUIActionArgs
{
    public FName ActionName => UBindUIActionArgsBlueprintLibrary.GetActionName(this);

    public bool ActionHasHoldMappings =>
        UBindUIActionArgsBlueprintLibrary.ActionHasHoldMappings(this);

    public FUIActionTag ActionTag
    {
        get => UBindUIActionArgsBlueprintLibrary.GetActionTag(this);
        init => UBindUIActionArgsBlueprintLibrary.SetActionTag(this, value);
    }

    public FDataTableRowHandle ActionTableRow
    {
        get => UBindUIActionArgsBlueprintLibrary.GetActionTableRow(this);
        init => UBindUIActionArgsBlueprintLibrary.SetActionTableRow(this, value);
    }

    public UInputAction? InputAction
    {
        get => UBindUIActionArgsBlueprintLibrary.GetInputAction(this);
        init => UBindUIActionArgsBlueprintLibrary.SetInputAction(this, value);
    }

    public ECommonInputMode InputMode
    {
        get => UBindUIActionArgsBlueprintLibrary.GetInputMode(this);
        init => UBindUIActionArgsBlueprintLibrary.SetInputMode(this, value);
    }

    public EInputEvent KeyEvent
    {
        get => UBindUIActionArgsBlueprintLibrary.GetKeyEvent(this);
        init => UBindUIActionArgsBlueprintLibrary.SetKeyEvent(this, value);
    }

    public IEnumerable<ECommonInputType> InputTypesExemptFromValidKeyCheck
    {
        get => UBindUIActionArgsBlueprintLibrary.GetInputTypesExemptFromValidKeyCheck(this);
        init
        {
            if (value is ISet<ECommonInputType> set)
            {
                UBindUIActionArgsBlueprintLibrary.SetInputTypesExemptFromValidKeyCheck(this, set);
            }
            else
            {
                UBindUIActionArgsBlueprintLibrary.SetInputTypesExemptFromValidKeyCheck(
                    this,
                    value.ToHashSet()
                );
            }
        }
    }

    public bool IsPersistent
    {
        get => UBindUIActionArgsBlueprintLibrary.GetIsPersistent(this);
        init => UBindUIActionArgsBlueprintLibrary.SetIsPersistent(this, value);
    }

    public bool ConsumeInput
    {
        get => UBindUIActionArgsBlueprintLibrary.GetConsumeInput(this);
        init => UBindUIActionArgsBlueprintLibrary.SetConsumeInput(this, value);
    }

    public bool DisplayInActionBar
    {
        get => UBindUIActionArgsBlueprintLibrary.GetConsumeInput(this);
        init => UBindUIActionArgsBlueprintLibrary.SetConsumeInput(this, value);
    }

    public bool ForceHold
    {
        get => UBindUIActionArgsBlueprintLibrary.GetForceHold(this);
        init => UBindUIActionArgsBlueprintLibrary.SetForceHold(this, value);
    }

    public FText OverrideDisplayName
    {
        get => UBindUIActionArgsBlueprintLibrary.GetOverrideDisplayName(this);
        init => UBindUIActionArgsBlueprintLibrary.SetOverrideDisplayName(this, value);
    }

    public int PriorityWithinCollection
    {
        get => UBindUIActionArgsBlueprintLibrary.GetPriorityWithinCollection(this);
        init => UBindUIActionArgsBlueprintLibrary.SetPriorityWithinCollection(this, value);
    }

    public OnExecuteActionDelegate OnExecuteAction
    {
        init
        {
            var ueDelegate = new TDelegate<OnExecuteActionDelegate>();
            ueDelegate.Add(value);
            UBindUIActionArgsBlueprintLibrary.SetOnExecuteAction(this, ueDelegate);
        }
    }

    public OnHoldActionProgressedDelegate OnHoldActionProgressed
    {
        init
        {
            var ueDelegate = new TDelegate<OnHoldActionProgressedDelegate>();
            ueDelegate.Add(value);
            UBindUIActionArgsBlueprintLibrary.SetOnHoldActionProgressed(this, ueDelegate);
        }
    }

    public OnHoldActionPressedDelegate OnHoldActionPressed
    {
        init
        {
            var ueDelegate = new TDelegate<OnHoldActionPressedDelegate>();
            ueDelegate.Add(value);
            UBindUIActionArgsBlueprintLibrary.SetOnHoldActionPressed(this, ueDelegate);
        }
    }

    public OnHoldActionReleasedDelegate OnHoldActionReleased
    {
        init
        {
            var ueDelegate = new TDelegate<OnHoldActionReleasedDelegate>();
            ueDelegate.Add(value);
            UBindUIActionArgsBlueprintLibrary.SetOnHoldActionReleased(this, ueDelegate);
        }
    }

    public FCSBindUIActionArgs(FUIActionTag actionTag, OnExecuteActionDelegate onExecuteAction)
    {
        var ueDelegate = new TDelegate<OnExecuteActionDelegate>();
        ueDelegate.Add(onExecuteAction);
        UBindUIActionArgsBlueprintLibrary.InitActionTag(ref this, actionTag, ueDelegate);
    }

    public FCSBindUIActionArgs(
        FUIActionTag actionTag,
        bool shouldDisplayInActionBar,
        OnExecuteActionDelegate onExecuteAction
    )
    {
        var ueDelegate = new TDelegate<OnExecuteActionDelegate>();
        ueDelegate.Add(onExecuteAction);
        UBindUIActionArgsBlueprintLibrary.InitActionTagBool(
            ref this,
            actionTag,
            shouldDisplayInActionBar,
            ueDelegate
        );
    }

    public FCSBindUIActionArgs(
        FDataTableRowHandle actionTableRow,
        OnExecuteActionDelegate onExecuteAction
    )
    {
        var ueDelegate = new TDelegate<OnExecuteActionDelegate>();
        ueDelegate.Add(onExecuteAction);
        UBindUIActionArgsBlueprintLibrary.InitDataTableRowHandle(
            ref this,
            actionTableRow,
            ueDelegate
        );
    }

    public FCSBindUIActionArgs(
        FDataTableRowHandle actionTableRow,
        bool shouldDisplayInActionBar,
        OnExecuteActionDelegate onExecuteAction
    )
    {
        var ueDelegate = new TDelegate<OnExecuteActionDelegate>();
        ueDelegate.Add(onExecuteAction);
        UBindUIActionArgsBlueprintLibrary.InitDataTableRowHandleBool(
            ref this,
            actionTableRow,
            shouldDisplayInActionBar,
            ueDelegate
        );
    }

    public FCSBindUIActionArgs(UInputAction inputAction, OnExecuteActionDelegate onExecuteAction)
    {
        var ueDelegate = new TDelegate<OnExecuteActionDelegate>();
        ueDelegate.Add(onExecuteAction);
        UBindUIActionArgsBlueprintLibrary.InitInputAction(ref this, inputAction, ueDelegate);
    }

    public FCSBindUIActionArgs(
        UInputAction inputAction,
        bool shouldDisplayInActionBar,
        OnExecuteActionDelegate onExecuteAction
    )
    {
        var ueDelegate = new TDelegate<OnExecuteActionDelegate>();
        ueDelegate.Add(onExecuteAction);
        UBindUIActionArgsBlueprintLibrary.InitInputActionBool(
            ref this,
            inputAction,
            shouldDisplayInActionBar,
            ueDelegate
        );
    }

    public FCSBindUIActionArgs(FCSBindUIActionArgs other)
    {
        this = UBindUIActionArgsBlueprintLibrary.Clone(other);
    }
}
