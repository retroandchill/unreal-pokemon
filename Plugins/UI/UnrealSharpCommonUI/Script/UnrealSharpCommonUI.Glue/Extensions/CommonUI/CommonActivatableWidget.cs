// ReSharper disable once CheckNamespace

using UnrealSharp.UnrealSharpCommonUI;

namespace UnrealSharp.CommonUI;

public partial class UCommonActivatableWidget
{
    public IList<FUIActionBindingHandle> ActionBindings => UCommonActivatableWidgetExtensions.GetActionBindings(this);

    public FUIActionBindingHandle RegisterUIActionBinding(FCSBindUIActionArgs args)
    {
        return UCommonActivatableWidgetExtensions.RegisterUIActionBinding(this, args);
    }

    public void AddActionBinding(FUIActionBindingHandle handle)
    {
        UCommonActivatableWidgetExtensions.AddActionBinding(this, handle);
    }

    public void RemoveActionBinding(FUIActionBindingHandle handle)
    {
        UCommonActivatableWidgetExtensions.RemoveActionBinding(this, handle);
    }
}