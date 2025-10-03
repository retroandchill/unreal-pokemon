using System.Runtime.CompilerServices;
using Microsoft.Extensions.DependencyInjection;
using UnrealSharp.Attributes;
using UnrealSharp.UnrealSharpCore;

namespace UnrealInject.Subsystems;

/// <summary>
/// Represents a game instance subsystem that acts as a dependency injection container
/// to manage and provide services for other subsystems within the game.
/// </summary>
/// <remarks>
/// This system integrates with Microsoft's dependency injection framework, providing
/// functionality for service registration, resolution, and scoping. It serves as a central
/// service provider for game-related subsystems, ensuring loose coupling and better testability.
/// </remarks>
[UClass]
public sealed class UDependencyInjectionGameInstanceSubsystem
    : UCSGameInstanceSubsystem,
        IServiceProvider,
        IServiceScopeFactory
{
    private IServiceProvider _serviceProvider = null!;

    /// <inheritdoc />
    protected override void Initialize(FSubsystemCollectionBaseRef collection)
    {
        _serviceProvider = FUnrealInjectModule.Instance.BuildServiceProvider();
    }

    /// <inheritdoc />
    protected override void Deinitialize()
    {
        if (_serviceProvider is IDisposable disposable)
        {
            disposable.Dispose();
        }
    }

    /// <inheritdoc />
    public object? GetService(Type serviceType)
    {
        return _serviceProvider.GetService(serviceType);
    }

    /// <inheritdoc />
    public IServiceScope CreateScope()
    {
        return _serviceProvider.CreateScope();
    }
}
