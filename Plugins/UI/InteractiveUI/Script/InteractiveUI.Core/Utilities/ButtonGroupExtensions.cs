using UnrealSharp.CommonUI;

namespace InteractiveUI.Core.Utilities;

/// <summary>
/// Provides extension methods for interacting with UCommonButtonGroupBase objects, enabling easier access and manipulation of buttons within groups.
/// </summary>
public static class ButtonGroupExtensions
{
    /// <summary>
    /// Retrieves a button of the specified type from the button group at the given index.
    /// </summary>
    /// <typeparam name="T">The type of button to retrieve. Must inherit from <see cref="UCommonButtonBase"/>.</typeparam>
    /// <param name="buttonGroup">The button group from which to retrieve the button.</param>
    /// <param name="index">The index of the button in the group.</param>
    /// <returns>
    /// The button at the specified index, cast to the specified type, or null if no button is found or it cannot be cast.
    /// </returns>
    public static T? GetButton<T>(this UCommonButtonGroupBase buttonGroup, int index)
        where T : UCommonButtonBase
    {
        return buttonGroup.GetButtonBaseAtIndex(index) as T;
    }

    /// <summary>
    /// Retrieves a button of the specified type from the button group at the given index, and ensures that the button exists.
    /// </summary>
    /// <typeparam name="T">The type of button to retrieve. Must inherit from <see cref="UCommonButtonBase"/>.</typeparam>
    /// <param name="buttonGroup">The button group from which to retrieve the button.</param>
    /// <param name="index">The index of the button in the group.</param>
    /// <returns>
    /// The button at the specified index, cast to the specified type.
    /// </returns>
    /// <exception cref="InvalidOperationException">
    /// Thrown if no button is found at the specified index or if it cannot be cast to the specified type.
    /// </exception>
    public static T GetRequiredButton<T>(this UCommonButtonGroupBase buttonGroup, int index)
        where T : UCommonButtonBase
    {
        return buttonGroup.GetButton<T>(index) ?? throw new InvalidOperationException("No button at index");
    }

    /// <summary>
    /// Retrieves all buttons of the specified type from the button group along with their corresponding indices.
    /// </summary>
    /// <typeparam name="T">The type of buttons to retrieve. Must inherit from <see cref="UCommonButtonBase"/>.</typeparam>
    /// <param name="buttonGroup">The button group from which to retrieve the buttons.</param>
    /// <returns>
    /// An enumerable collection of tuples, where each tuple contains the index and the button of the specified type.
    /// Only buttons that are not null and match the specified type are included.
    /// </returns>
    public static IEnumerable<(int Index, T Button)> GetButtons<T>(this UCommonButtonGroupBase buttonGroup)
        where T : UCommonButtonBase
    {
        for (var i = 0; i < buttonGroup.ButtonCount; i++)
        {
            var button = buttonGroup.GetButton<T>(i);
            if (button is not null)
            {
                yield return (i, button);
            }
        }
    }
}
