using UnrealSharp.CoreUObject;
using UnrealSharp.DeveloperSettings;

namespace UnrealInject.Settings;

/// <summary>
/// Represents the base interface for obtaining developer settings in the application.
/// </summary>
/// <typeparam name="T">
/// The type of developer settings, which must derive from <see cref="UDeveloperSettings"/>.
/// </typeparam>
public interface IDeveloperSettings<out T>
    where T : UDeveloperSettings
{
    /// <summary>
    /// Gets the developer settings instance of type <typeparamref name="T"/>.
    /// </summary>
    /// <typeparam name="T">
    /// The specific type of developer settings, constrained to derive from <see cref="UDeveloperSettings"/>.
    /// </typeparam>
    /// <remarks>
    /// This property provides access to the default settings for developers in the application. The settings are retrieved
    /// using the Unreal Engine mechanism for accessing default object instances.
    /// </remarks>
    T Settings { get; }
}

internal sealed class DeveloperSettings<T> : IDeveloperSettings<T>
    where T : UDeveloperSettings
{
    public T Settings { get; } = UObject.GetDefault<T>();
}
