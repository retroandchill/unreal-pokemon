using UnrealSharp.CommonUI;
using ZLinq;

namespace InteractiveUI.Core.Utilities;

public static class FocusUtilities
{
    public static UCommonActivatableWidget? GetFirstActivatedWidget(
        params ReadOnlySpan<UCommonActivatableWidget> widgets
    )
    {
        return widgets.AsValueEnumerable().FirstOrDefault(w => w.IsActive);
    }

    public static UCommonActivatableWidget? GetFirstActivatedWidget(
        IEnumerable<UCommonActivatableWidget> widgets
    )
    {
        return widgets.FirstOrDefault(w => w.IsActive);
    }
}
