using UnrealSharp;
using UnrealSharp.Binds;

namespace GameDataAccessTools.Core.Interop;

/// <summary>
/// Provides functionality for managing localized text resources and creating localized text instances.
/// This class serves as a static utility for interacting with native string localization data used
/// in Unreal Engine-based systems.
/// </summary>
[NativeCallbacks]
public static unsafe partial class LocalizationExporter
{
    private static readonly delegate* unmanaged<char*, char*, char*, out FTextData, void> CreateLocalizedText;

    private static readonly delegate* unmanaged<char*, out FTextData, void> FromLocalizedString;
}
