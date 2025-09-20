using UnrealSharp.CommonUI;

namespace InteractiveUI.Core.Utilities;

public static class ButtonGroupExtensions
{
    public static T? GetButton<T>(this UCommonButtonGroupBase buttonGroup, int index)
        where T : UCommonButtonBase
    {
        return buttonGroup.GetButtonBaseAtIndex(index) as T;
    }

    public static T GetRequiredButton<T>(this UCommonButtonGroupBase buttonGroup, int index)
        where T : UCommonButtonBase
    {
        return buttonGroup.GetButton<T>(index)
            ?? throw new InvalidOperationException("No button at index");
    }

    public static IEnumerable<(int Index, T Button)> GetButtons<T>(
        this UCommonButtonGroupBase buttonGroup
    )
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
