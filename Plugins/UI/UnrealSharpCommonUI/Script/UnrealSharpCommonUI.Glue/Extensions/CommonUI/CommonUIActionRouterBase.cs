using UnrealSharp.UMG;
using UnrealSharp.UnrealSharpCommonUI;

namespace UnrealSharp.CommonUI;

public partial class UCommonUIActionRouterBase
{
    public FUIActionBindingHandle RegisterUIActionBinding(UWidget widget, FCSBindUIActionArgs args)
    {
        return UActionRouterExtensions.RegisterUIActionBinding(this, widget, args);
    }
}