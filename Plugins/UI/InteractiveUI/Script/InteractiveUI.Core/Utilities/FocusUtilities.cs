using UnrealSharp.CommonUI;
using ZLinq;

namespace InteractiveUI.Core.Utilities;

/// <summary>
/// Provides utility methods for working with focusable widgets within a UI context.
/// </summary>
public static class FocusUtilities
{
    /// <summary>
    /// Retrieves the first activated widget from a sequence of widgets.
    /// A widget is considered activated if its <c>IsActive</c> property is <c>true</c>.
    /// </summary>
    /// <param name="widgets">
    /// A sequence of <see cref="UCommonActivatableWidget"/> to search for an active widget.
    /// </param>
    /// <returns>
    /// The first widget in the sequence with <c>IsActive</c> set to <c>true</c>, or <c>null</c> if no such widget is found.
    /// </returns>
    public static UCommonActivatableWidget? GetFirstActivatedWidget(
        params ReadOnlySpan<UCommonActivatableWidget> widgets
    )
    {
        return widgets.AsValueEnumerable().FirstOrDefault(w => w.IsActive);
    }

    /// <summary>
    /// Retrieves the first activated widget from a collection, where a widget is considered activated
    /// if its <c>IsActive</c> property is <c>true</c>.
    /// </summary>
    /// <param name="widgets">
    /// A collection of <see cref="UCommonActivatableWidget"/> to search for an active widget.
    /// </param>
    /// <returns>
    /// The first <see cref="UCommonActivatableWidget"/> in the collection with <c>IsActive</c> set to <c>true</c>,
    /// or <c>null</c> if no such widget is found.
    /// </returns>
    public static UCommonActivatableWidget? GetFirstActivatedWidget(IEnumerable<UCommonActivatableWidget> widgets)
    {
        return widgets.FirstOrDefault(w => w.IsActive);
    }
}
