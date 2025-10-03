using UnrealSharp.CommonInput;
using UnrealSharp.CommonUI;
using UnrealSharp.Engine;
using UnrealSharp.EnhancedInput;

namespace UnrealSharp.UnrealSharpCommonUI;

/// <summary>
/// Represents a structure for defining properties and behaviors of UI action bindings in the Unreal Sharp Common UI system.
/// </summary>
public partial record struct FCSBindUIActionArgs
{
    /// <summary>
    /// Gets the name of the UI action associated with the current instance.
    /// This property retrieves a unique identifier (of type FName) for the UI action
    /// that is being represented by the current object. The name can be used to
    /// distinguish between different actions in the system or for debugging purposes.
    /// </summary>
    public FName ActionName => UBindUIActionArgsBlueprintLibrary.GetActionName(this);

    /// <summary>
    /// Indicates whether the associated UI action has mappings that require a hold
    /// interaction to be triggered. This property checks if any of the input mappings
    /// for the action are configured to require a hold action from the user.
    /// </summary>
    public bool ActionHasHoldMappings => UBindUIActionArgsBlueprintLibrary.ActionHasHoldMappings(this);

    /// <summary>
    /// Gets or initializes the action tag associated with the current instance.
    /// This property represents a unique identifier of type FUIActionTag that is
    /// used to categorize or label the UI action. It can be utilized for functionality
    /// association, filtering, or identification of the specific action across the system.
    /// </summary>
    public FUIActionTag ActionTag
    {
        get => UBindUIActionArgsBlueprintLibrary.GetActionTag(this);
        init => UBindUIActionArgsBlueprintLibrary.SetActionTag(this, value);
    }

    /// <summary>
    /// Gets or sets the data table row handle associated with the current UI action.
    /// This property represents a reference to a specific entry in a data table, which
    /// provides additional configuration or metadata for the associated UI action.
    /// Modifying this property updates the reference through the underlying framework.
    /// </summary>
    public FDataTableRowHandle ActionTableRow
    {
        get => UBindUIActionArgsBlueprintLibrary.GetActionTableRow(this);
        init => UBindUIActionArgsBlueprintLibrary.SetActionTableRow(this, value);
    }

    /// <summary>
    /// Represents the specific input action associated with the current instance.
    /// This property allows for getting or setting an input action of type UInputAction,
    /// which defines the interaction or input event the action corresponds to.
    /// </summary>
    public UInputAction? InputAction
    {
        get => UBindUIActionArgsBlueprintLibrary.GetInputAction(this);
        init => UBindUIActionArgsBlueprintLibrary.SetInputAction(this, value);
    }

    /// <summary>
    /// Represents the input mode used for the current UI action.
    /// This property determines how the input is managed and interpreted
    /// within the context of the user interface, such as whether it operates
    /// in game mode, UI mode, or a hybrid of both.
    /// </summary>
    public ECommonInputMode InputMode
    {
        get => UBindUIActionArgsBlueprintLibrary.GetInputMode(this);
        init => UBindUIActionArgsBlueprintLibrary.SetInputMode(this, value);
    }

    /// <summary>
    /// Gets or sets the specific input key event type associated with the current instance.
    /// This property represents an event state for an input, such as key press, release, or repeat,
    /// providing granular control over input handling in a UI action binding.
    /// </summary>
    public EInputEvent KeyEvent
    {
        get => UBindUIActionArgsBlueprintLibrary.GetKeyEvent(this);
        init => UBindUIActionArgsBlueprintLibrary.SetKeyEvent(this, value);
    }

    /// <summary>
    /// Gets or sets the input types that are exempt from the valid key check when binding a UI action.
    /// This property specifies a collection of input types (of type ECommonInputType) which do not require
    /// validation for key bindings. This is particularly useful for system-defined input types or special cases
    /// where input validation is not necessary.
    /// </summary>
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
                UBindUIActionArgsBlueprintLibrary.SetInputTypesExemptFromValidKeyCheck(this, value.ToHashSet());
            }
        }
    }

    /// <summary>
    /// Indicates whether the associated action is persistent across application states or contexts.
    /// A persistent action remains active or valid even when transitioning between different
    /// sections or modes of the application, ensuring consistency in its availability.
    /// </summary>
    public bool IsPersistent
    {
        get => UBindUIActionArgsBlueprintLibrary.GetIsPersistent(this);
        init => UBindUIActionArgsBlueprintLibrary.SetIsPersistent(this, value);
    }

    /// <summary>
    /// Indicates whether the input associated with the action should be consumed.
    /// When set to true, the input for this action is marked as handled, preventing it
    /// from being further processed by other systems. This property is typically used
    /// to control the propagation of input events in the input handling pipeline.
    /// </summary>
    public bool ConsumeInput
    {
        get => UBindUIActionArgsBlueprintLibrary.GetConsumeInput(this);
        init => UBindUIActionArgsBlueprintLibrary.SetConsumeInput(this, value);
    }

    /// <summary>
    /// Determines whether the UI action associated with the current instance should be displayed in the action bar.
    /// This property can be used to specify or retrieve the visibility status of the action within the action bar,
    /// allowing for customization of user interface actions based on user or system requirements.
    /// </summary>
    public bool DisplayInActionBar
    {
        get => UBindUIActionArgsBlueprintLibrary.GetConsumeInput(this);
        init => UBindUIActionArgsBlueprintLibrary.SetConsumeInput(this, value);
    }

    /// <summary>
    /// Determines whether the associated UI action requires a hold input to activate.
    /// This property indicates if the action must be executed by holding the input
    /// for a certain duration, as opposed to a single press. It can be used for actions
    /// that require deliberate or extended input for confirmation or execution.
    /// </summary>
    public bool ForceHold
    {
        get => UBindUIActionArgsBlueprintLibrary.GetForceHold(this);
        init => UBindUIActionArgsBlueprintLibrary.SetForceHold(this, value);
    }

    /// <summary>
    /// Gets or sets the override display name for the associated action.
    /// This property specifies a custom text label (of type FText) that can be used
    /// to provide a more specific or localized display name for the action,
    /// overriding any default or system-generated names.
    /// </summary>
    public FText OverrideDisplayName
    {
        get => UBindUIActionArgsBlueprintLibrary.GetOverrideDisplayName(this);
        init => UBindUIActionArgsBlueprintLibrary.SetOverrideDisplayName(this, value);
    }

    /// <summary>
    /// Gets or sets the priority level of the current UI action within its collection.
    /// This property determines the relative importance or execution order of the action
    /// when multiple actions are part of the same context or collection. A higher value
    /// may indicate a higher priority, affecting how the action is processed or displayed.
    /// </summary>
    public int PriorityWithinCollection
    {
        get => UBindUIActionArgsBlueprintLibrary.GetPriorityWithinCollection(this);
        init => UBindUIActionArgsBlueprintLibrary.SetPriorityWithinCollection(this, value);
    }

    /// <summary>
    /// Defines the delegate that executes the associated UI action.
    /// This property is responsible for setting up a callback function which
    /// will be invoked when the corresponding UI action is executed by the user.
    /// It allows the system to bind specific functionality to the action.
    /// </summary>
    public OnExecuteActionDelegate OnExecuteAction
    {
        init
        {
            var ueDelegate = new TDelegate<OnExecuteActionDelegate>();
            ueDelegate.Add(value);
            UBindUIActionArgsBlueprintLibrary.SetOnExecuteAction(this, ueDelegate);
        }
    }

    /// <summary>
    /// Represents a delegate that is triggered as the progress of a hold action is updated.
    /// The delegate receives a float value indicating the current progress of the hold,
    /// where 0 represents no progress and 1 represents the hold action being fully completed.
    /// </summary>
    public OnHoldActionProgressedDelegate OnHoldActionProgressed
    {
        init
        {
            var ueDelegate = new TDelegate<OnHoldActionProgressedDelegate>();
            ueDelegate.Add(value);
            UBindUIActionArgsBlueprintLibrary.SetOnHoldActionProgressed(this, ueDelegate);
        }
    }

    /// <summary>
    /// Specifies the delegate to be executed when the associated hold-based UI action is triggered.
    /// This property allows the assignment of a callback function that will respond
    /// when a hold action event occurs, commonly used in interactive UI contexts.
    /// </summary>
    public OnHoldActionPressedDelegate OnHoldActionPressed
    {
        init
        {
            var ueDelegate = new TDelegate<OnHoldActionPressedDelegate>();
            ueDelegate.Add(value);
            UBindUIActionArgsBlueprintLibrary.SetOnHoldActionPressed(this, ueDelegate);
        }
    }

    /// <summary>
    /// Defines a delegate to be invoked when the hold action associated with the current UI action is released.
    /// This delegate enables the subscription of custom logic or event handlers for scenarios where a hold action
    /// is completed or cancelled by the user.
    /// </summary>
    public OnHoldActionReleasedDelegate OnHoldActionReleased
    {
        init
        {
            var ueDelegate = new TDelegate<OnHoldActionReleasedDelegate>();
            ueDelegate.Add(value);
            UBindUIActionArgsBlueprintLibrary.SetOnHoldActionReleased(this, ueDelegate);
        }
    }

    /// <summary>
    /// Represents a struct used to bind user interface actions in Unreal Engine.
    /// </summary>
    /// <param name="actionTag">The action tag associated with the UI action.</param>
    /// <param name="onExecuteAction">The delegate to be executed when the action is triggered.</param>
    /// <remarks>
    /// FCSBindUIActionArgs provides a comprehensive way to define and configure UI actions,
    /// including action names, tags, input settings, and execution logic.
    /// </remarks>
    public FCSBindUIActionArgs(FUIActionTag actionTag, OnExecuteActionDelegate onExecuteAction)
    {
        var ueDelegate = new TDelegate<OnExecuteActionDelegate>();
        ueDelegate.Add(onExecuteAction);
        UBindUIActionArgsBlueprintLibrary.InitActionTag(ref this, actionTag, ueDelegate);
    }

    /// <summary>
    /// Represents a struct used to bind user interface actions in Unreal Engine with customizable configurations and behaviors.
    /// </summary>
    /// <param name="actionTag">The action tag associated with the UI action.</param>
    /// <param name="shouldDisplayInActionBar">Indicates whether the action should be displayed in the action bar.</param>
    /// <param name="onExecuteAction">The delegate to be executed when the action is triggered.</param>
    /// <remarks>
    /// FCSBindUIActionArgs provides a versatile framework for managing UI actions by defining input actions, action tags,
    /// data table rows, and execution delegates. It also supports additional parameters like persistence, input mode,
    /// and exemptions from valid key checks, offering flexibility in configuring UI interactions.
    /// </remarks>
    public FCSBindUIActionArgs(
        FUIActionTag actionTag,
        bool shouldDisplayInActionBar,
        OnExecuteActionDelegate onExecuteAction
    )
    {
        var ueDelegate = new TDelegate<OnExecuteActionDelegate>();
        ueDelegate.Add(onExecuteAction);
        UBindUIActionArgsBlueprintLibrary.InitActionTagBool(ref this, actionTag, shouldDisplayInActionBar, ueDelegate);
    }

    /// <summary>
    /// Represents a struct used to configure and bind user interface actions in Unreal Engine.
    /// </summary>
    /// <param name="actionTableRow">The data table row handle associated with the UI action.</param>
    /// <param name="onExecuteAction">The delegate to be executed when the action is triggered.</param>
    /// <remarks>
    /// FCSBindUIActionArgs encompasses various parameters such as action tags, input actions,
    /// key events, and display settings to streamline the definition and execution of UI interactions.
    /// It allows for customization of input modes and validation of input types, enabling flexible
    /// integration of UI-related functionalities.
    /// </remarks>
    public FCSBindUIActionArgs(FDataTableRowHandle actionTableRow, OnExecuteActionDelegate onExecuteAction)
    {
        var ueDelegate = new TDelegate<OnExecuteActionDelegate>();
        ueDelegate.Add(onExecuteAction);
        UBindUIActionArgsBlueprintLibrary.InitDataTableRowHandle(ref this, actionTableRow, ueDelegate);
    }

    /// <summary>
    /// Represents a record struct used to define and configure bindings for user interface actions in Unreal Engine.
    /// </summary>
    /// <param name="actionTableRow">The data table row handle associated with the UI action.</param>
    /// <param name="shouldDisplayInActionBar">Indicates whether the action should be displayed in the action bar.</param>
    /// <param name="onExecuteAction">The delegate to be executed when the action is triggered.</param>
    /// <remarks>
    /// FCSBindUIActionArgs allows developers to specify action-related properties such as action tags, input events,
    /// persistence, and input action mappings. Additionally, it provides constructors for different use cases, enabling
    /// detailed configuration of UI actions, including their execution logic and display settings.
    /// </remarks>
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

    /// <summary>
    /// Represents a struct used to define and configure user interface actions in UnrealSharp-based systems.
    /// </summary>
    /// <param name="inputAction">The input action associated with the UI action.</param>
    /// <param name="onExecuteAction">The delegate to be executed when the action is triggered.</param>
    /// <remarks>
    /// FCSBindUIActionArgs provides a flexible way to bind UI actions, managing properties such as action tags,
    /// input actions, input modes, key events, persistence settings, and execution delegates.
    /// It encapsulates multiple construction methods, facilitating the configuration of actions with varying initialization requirements.
    /// </remarks>
    public FCSBindUIActionArgs(UInputAction inputAction, OnExecuteActionDelegate onExecuteAction)
    {
        var ueDelegate = new TDelegate<OnExecuteActionDelegate>();
        ueDelegate.Add(onExecuteAction);
        UBindUIActionArgsBlueprintLibrary.InitInputAction(ref this, inputAction, ueDelegate);
    }

    /// <summary>
    /// Represents a struct utilized for binding UI actions in Unreal Engine.
    /// </summary>
    /// <param name="onExecuteAction">The delegate executed when the UI action is triggered.</param>
    /// <param name="shouldDisplayInActionBar">Indicates whether the action should be visible in the action bar.</param>
    /// <param name="inputAction">An input action that triggers the bound UI action.</param>
    /// <remarks>
    /// FCSBindUIActionArgs allows for the initialization and handling of user interface actions
    /// within Unreal Engine by specifying execution logic, input mappings, and optional UI configuration.
    /// This struct simplifies the process of setting up UI actions by supporting key action details
    /// and integrating with the Common UI framework.
    /// </remarks>
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

    /// <summary>
    /// Represents a record struct designed to encapsulate the configuration and functionality
    /// for binding user interface actions in Unreal Engine's Common UI system.
    /// </summary>
    /// <param name="other">The other instance of the struct to be cloned.</param>
    /// <remarks>
    /// FCSBindUIActionArgs allows the detailed specification of UI actions,
    /// including action tags, input settings, persistence, and execution logic.
    /// It supports initialization through multiple constructors to accommodate different binding scenarios
    /// and leverages helper functions from UBindUIActionArgsBlueprintLibrary for internal data handling.
    /// </remarks>
    public FCSBindUIActionArgs(FCSBindUIActionArgs other)
    {
        this = UBindUIActionArgsBlueprintLibrary.Clone(other);
    }
}
