using GameDataAccessTools.Core.Interop;
using UnrealSharp;
using UnrealSharp.Interop;

namespace GameDataAccessTools.Core.Utilities;

/// <summary>
/// Provides extension methods for localization-related functionalities.
/// </summary>
public static class LocalizationExtensions
{
    /// <summary>
    /// Extensions for FText.
    /// </summary>
    extension(FText)
    {
        /// <summary>
        /// Creates a localized <see cref="FText"/> instance based on the provided namespace, key, and default value.
        /// </summary>
        /// <param name="ns">The namespace to use for localization. Defines the category or context of the text.</param>
        /// <param name="key">The key within the namespace that identifies the text for localization.</param>
        /// <param name="defaultValue">The fallback text to use if the localized text is not found.</param>
        /// <returns>A localized <see cref="FText"/> instance corresponding to the given parameters.</returns>
        public static FText Localized(ReadOnlySpan<char> ns, ReadOnlySpan<char> key, ReadOnlySpan<char> defaultValue)
        {
            unsafe
            {
                FTextData textData;
                fixed (char* nsPtr = ns)
                fixed (char* keyPtr = key)
                fixed (char* defaultPtr = defaultValue)
                {
                    LocalizationExporter.CallCreateLocalizedText(nsPtr, keyPtr, defaultPtr, out textData);
                }

                var result = TextMarshaller.FromNative((IntPtr)(&textData), 0);
                textData.ObjectPointer.Release();
                return result;
            }
        }

        /// <summary>
        /// Converts a localized string into an <see cref="FText"/> instance.
        /// </summary>
        /// <param name="localizedString">The string representing a localized text, typically retrieved or constructed for localization purposes.</param>
        /// <returns>An <see cref="FText"/> instance created from the provided localized string.</returns>
        public static FText FromLocalizedString(ReadOnlySpan<char> localizedString)
        {
            unsafe
            {
                fixed (char* ptr = localizedString)
                {
                    LocalizationExporter.CallFromLocalizedString(ptr, out var textData);
                    var result = TextMarshaller.FromNative((IntPtr)(&textData), 0);
                    textData.ObjectPointer.Release();
                    return result;
                }
            }
        }
    }
}
