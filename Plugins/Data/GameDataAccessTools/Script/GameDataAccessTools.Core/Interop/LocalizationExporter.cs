using UnrealSharp;
using UnrealSharp.Binds;

namespace GameDataAccessTools.Core.Interop;

[NativeCallbacks]
public static unsafe partial class LocalizationExporter
{
    private static readonly delegate* unmanaged<
        char*,
        char*,
        char*,
        out FTextData,
        void> CreateLocalizedText;

    private static readonly delegate* unmanaged<char*, out FTextData, void> FromLocalizedString;
}
