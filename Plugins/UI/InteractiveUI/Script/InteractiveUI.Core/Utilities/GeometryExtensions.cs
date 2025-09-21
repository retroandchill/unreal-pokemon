using UnrealSharp.CoreUObject;
using UnrealSharp.SlateCore;
using UnrealSharp.UMG;

namespace InteractiveUI.Core.Utilities;

public static class GeometryExtensions
{
    extension(FGeometry geometry)
    {
        public FVector2D LocalSize => SlateLibrary.GetLocalSize(geometry);

        public FVector2D LocalTopLeft => SlateLibrary.GetLocalTopLeft(geometry);
    }
}
