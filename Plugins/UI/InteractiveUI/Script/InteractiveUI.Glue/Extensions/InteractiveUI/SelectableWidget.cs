using UnrealSharp.CommonUI;

namespace UnrealSharp.InteractiveUI;

public partial class USelectableWidget
{
    public T? GetSelectableOption<T>(int optionIndex)
        where T : UCommonButtonBase
    {
        return GetSelectableOption(optionIndex) as T;
    }

    public IEnumerable<T> GetSelectableOptions<T>()
        where T : UCommonButtonBase
    {
        return SelectableButtons.Cast<T>();
    }
}
