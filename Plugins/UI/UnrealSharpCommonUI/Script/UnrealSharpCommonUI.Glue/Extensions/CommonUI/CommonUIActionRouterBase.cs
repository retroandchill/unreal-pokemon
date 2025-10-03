using UnrealSharp.UMG;
using UnrealSharp.UnrealSharpCommonUI;

namespace UnrealSharp.CommonUI;

public partial class UCommonUIActionRouterBase
{
    /// Registers a UI action binding for the specified widget and action arguments.
    /// </summary>
    /// <param name="widget">The widget to bind the action to.</param>
    /// <param name="args">The binding arguments containing action details.</param>
    /// <returns>Returns a handle to the registered UI action binding.</returns>
    public FUIActionBindingHandle RegisterUIActionBinding(UWidget widget, FCSBindUIActionArgs args)
    {
        return UActionRouterExtensions.RegisterUIActionBinding(this, widget, args);
    }
}
