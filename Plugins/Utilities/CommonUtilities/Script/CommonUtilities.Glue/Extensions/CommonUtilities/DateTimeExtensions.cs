using UnrealSharp.CoreUObject;

namespace UnrealSharp.CommonUtilities;

/// <summary>
/// Provides extension methods for DateTime manipulation and operations.
/// </summary>
public static class DateTimeExtensions
{
    /// <summary>
    /// Extensions for FDateTime.
    /// </summary>
    extension(FDateTime)
    {
        /// <summary>
        /// A property to retrieve the current date and time as an FDateTime
        /// object using the UTimeBlueprintLibrary.DateTimeNow functionality.
        /// </summary>
        /// <remarks>
        /// This property serves as an extension for the FDateTime type
        /// and provides the current system date and time.
        /// </remarks>
        public static FDateTime Now => UTimeBlueprintLibrary.DateTimeNow;
    }
}
