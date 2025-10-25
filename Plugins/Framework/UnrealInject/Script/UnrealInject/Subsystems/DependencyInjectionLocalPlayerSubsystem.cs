using Microsoft.Extensions.DependencyInjection;
using UnrealSharp.Attributes;
using UnrealSharp.UnrealSharpCore;

namespace UnrealInject.Subsystems;

/// <summary>
/// Represents a local player subsystem that integrates with a dependency injection container
/// and provides scoped service resolution for local player-specific contexts.
/// </summary>
/// <remarks>
/// This class extends the <see cref="UCSLocalPlayerSubsystem"/> and implements the
/// <see cref="IServiceProvider"/> and <see cref="IServiceScope"/> interfaces to manage
/// and resolve services for the local player context. It relies on a scoped dependency
/// injection model provided by the parent game instance subsystem.
/// </remarks>
/// <seealso cref="UDependencyInjectionGameInstanceSubsystem"/>
[UClass]
public partial class UDependencyInjectionLocalPlayerSubsystem : UCSLocalPlayerSubsystem, IServiceProvider, IServiceScope
{
    private IServiceScope _serviceScope = null!;

    /// <inheritdoc />
    public IServiceProvider ServiceProvider => _serviceScope.ServiceProvider;

    /// <inheritdoc />
    protected override void Initialize_Implementation(FSubsystemCollectionBaseRef collection)
    {
        var gameInstanceSubsystem = GetGameInstanceSubsystem<UDependencyInjectionGameInstanceSubsystem>();
        _serviceScope = gameInstanceSubsystem.CreateScope();
    }

    /// <inheritdoc />
    protected override void Deinitialize_Implementation()
    {
        if (_serviceScope is IDisposable disposable)
        {
            disposable.Dispose();
        }
    }

    /// <inheritdoc />
    public object? GetService(Type serviceType)
    {
        return ServiceProvider.GetService(serviceType);
    }
}
