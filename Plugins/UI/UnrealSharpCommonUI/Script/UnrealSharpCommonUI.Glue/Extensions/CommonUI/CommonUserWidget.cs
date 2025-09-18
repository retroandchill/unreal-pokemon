// ReSharper disable once CheckNamespace

using UnrealSharp.UnrealSharpCommonUI;

namespace UnrealSharp.CommonUI;

public partial class UCommonUserWidget
{
    public IList<FUIActionBindingHandle> ActionBindings =>
        UCommonUserWidgetExtensions.GetActionBindings(this);

    public FUIActionBindingHandle RegisterUIActionBinding(FCSBindUIActionArgs args)
    {
        return UCommonUserWidgetExtensions.RegisterUIActionBinding(this, args);
    }

    public void AddActionBinding(FUIActionBindingHandle handle)
    {
        UCommonUserWidgetExtensions.AddActionBinding(this, handle);
    }

    public void RemoveActionBinding(FUIActionBindingHandle handle)
    {
        UCommonUserWidgetExtensions.RemoveActionBinding(this, handle);
    }
}
