using UnrealSharp.Engine;
using UnrealSharp.Test.Utils.Interop;
using UnrealSharp.UMG;

namespace UnrealSharp.Test.Utils;

/// <summary>
/// Defines a mechanism for managing the lifecycle of an instance
/// that should be automatically disposed when the scope ends.
/// </summary>
/// <typeparam name="T">The type of the scoped instance, which must be covariant.</typeparam>
public interface IScopedInstance<out T> : IDisposable
{
    /// <summary>
    /// Gets the instance managed by the scoped instance.
    /// </summary>
    /// <value>
    /// The specific instance of type <typeparamref name="T"/> that is being managed.
    /// </value>
    T Instance { get; }
}

/// <summary>
/// Represents a scoped widget that automatically removes itself from its parent when disposed.
/// </summary>
/// <typeparam name="T">The type of the widget, which must derive from UWidget.</typeparam>
public readonly record struct ScopedWidget<T>(T Instance) : IScopedInstance<T>
    where T : UWidget
{
    /// <inheritdoc />
    public void Dispose()
    {
        Instance.RemoveFromParent();
    }
}

/// <summary>
/// Represents a scoped local player that manages its lifecycle and ensures it
/// is removed from the appropriate context when disposed.
/// </summary>
public readonly record struct ScopedLocalPlayer(ULocalPlayer Instance)
    : IScopedInstance<ULocalPlayer>
{
    public void Dispose()
    {
        ScopedInstancesExporter.CallRemovePlayer(Instance.NativeObject);
    }
}
