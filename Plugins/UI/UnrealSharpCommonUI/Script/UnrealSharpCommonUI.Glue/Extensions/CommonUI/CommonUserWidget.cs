// ReSharper disable once CheckNamespace

using UnrealSharp.UnrealSharpCommonUI;

namespace UnrealSharp.CommonUI;

public partial class UCommonUserWidget
{
    /// <summary>
    /// Gets the collection of UI action bindings associated with the current widget.
    /// </summary>
    /// <remarks>
    /// The <c>ActionBindings</c> property represents a list of action bindings linked to the widget,
    /// allowing for interaction and customization of actions within the UI context.
    /// </remarks>
    public IList<FUIActionBindingHandle> ActionBindings => UCommonUserWidgetExtensions.GetActionBindings(this);

    /// <summary>
    /// Registers a user interface action binding for the provided widget.
    /// This method allows binding a specific action to a user interface element,
    /// enabling functionality to be invoked when the action is triggered.
    /// </summary>
    /// <param name="args">The arguments used for binding the action, including action details and handlers.</param>
    /// <returns>A handle representing the registered UI action binding, which can be used for handling or unbinding the action.</returns>
    public FUIActionBindingHandle RegisterUIActionBinding(FCSBindUIActionArgs args)
    {
        return UCommonUserWidgetExtensions.RegisterUIActionBinding(this, args);
    }

    /// <summary>
    /// Adds a new action binding to the user interface.
    /// This method associates a specific UI action binding handle with the widget,
    /// enabling its usage within the UI context.
    /// </summary>
    /// <param name="handle">The action binding handle to be added, representing the UI action and its configuration.</param>
    public void AddActionBinding(FUIActionBindingHandle handle)
    {
        UCommonUserWidgetExtensions.AddActionBinding(this, handle);
    }

    /// <summary>
    /// Removes an existing action binding from the user interface widget.
    /// This method is used to unbind a specific action previously registered,
    /// disabling its associated functionality and cleanup of resources.
    /// </summary>
    /// <param name="handle">The handle representing the action binding to be removed.</param>
    public void RemoveActionBinding(FUIActionBindingHandle handle)
    {
        UCommonUserWidgetExtensions.RemoveActionBinding(this, handle);
    }
}
