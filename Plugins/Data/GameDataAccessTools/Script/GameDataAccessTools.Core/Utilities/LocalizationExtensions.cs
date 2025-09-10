using GameDataAccessTools.Core.Interop;
using UnrealSharp;
using UnrealSharp.Interop;

namespace GameDataAccessTools.Core.Utilities;

public static class LocalizationExtensions
{
    extension(FText)
    {
        public static FText Localized(
            ReadOnlySpan<char> ns,
            ReadOnlySpan<char> key,
            ReadOnlySpan<char> defaultValue
        )
        {
            unsafe
            {
                FTextData textData;
                fixed (char* nsPtr = ns)
                fixed (char* keyPtr = key)
                fixed (char* defaultPtr = defaultValue)
                {
                    LocalizationExporter.CallCreateLocalizedText(
                        nsPtr,
                        keyPtr,
                        defaultPtr,
                        out textData
                    );
                }

                var result = TextMarshaller.FromNative((IntPtr)(&textData), 0);
                textData.ObjectPointer.Release();
                return result;
            }
        }

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
