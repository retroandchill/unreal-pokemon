using UnrealSharp.CoreUObject;
using UnrealSharp.SlateCore;
using UnrealSharp.UMG;

namespace InteractiveUI.Core.Utilities;

/// <summary>
/// Provides a collection of extension methods for geometric calculations and operations.
/// </summary>
public static class GeometryExtensions
{
    /// <summary>
    /// Provided extension methods for FGeometry.
    /// </summary>
    extension(FGeometry geometry)
    {
        /// <summary>
        /// Represents the local size of the geometry in 2D space.
        /// This property retrieves the size of the geometry in local coordinates,
        /// providing the width (X) and height (Y) as a 2D vector.
        /// </summary>
        public FVector2D LocalSize => SlateLibrary.GetLocalSize(geometry);

        /// <summary>
        /// Represents the local top-left position of the geometry in 2D space.
        /// This property retrieves the top-left corner of the geometry in local coordinates,
        /// providing its position as a 2D vector relative to its local space.
        /// </summary>
        public FVector2D LocalTopLeft => SlateLibrary.GetLocalTopLeft(geometry);
    }
}
