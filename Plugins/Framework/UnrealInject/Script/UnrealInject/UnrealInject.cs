using JetBrains.Annotations;
using Microsoft.Extensions.DependencyInjection;
using Retro.ReadOnlyParams.Annotations;
using UnrealInject.Settings;
using UnrealSharp.Engine.Core.Modules;

namespace UnrealInject;

#if WITH_EDITOR
/// <summary>
/// Represents an override for the game instance service provider, allowing a custom <see cref="IServiceProvider"/>
/// to be set and used within the Unreal Engine context.
/// </summary>
/// <remarks>
/// This class facilitates the customization of service provider logic for Unreal Engine game instances, primarily
/// for advanced dependency injection scenarios. When instantiated, it sets the custom service provider to the
/// Unreal Engine module. When disposed, it removes the override and reverts to the default behavior.
/// </remarks>
public sealed class GameInstanceServiceProviderOverride : IDisposable
{
    /// <summary>
    /// Provides functionality to override the default game instance service provider within the Unreal Engine module.
    /// </summary>
    /// <remarks>
    /// This class allows a custom implementation of <see cref="IServiceProvider"/> to temporarily replace the current
    /// service provider for a game instance. It is typically used for advanced customization or testing scenarios. When
    /// disposed, the override is removed, and the default behavior is restored.
    /// </remarks>
    public GameInstanceServiceProviderOverride(IServiceProvider serviceProvider)
    {
        FUnrealInjectModule.Instance.GameInstanceServiceProviderOverride = serviceProvider;
    }

    /// <inheritdoc />
    public void Dispose()
    {
        FUnrealInjectModule.Instance.GameInstanceServiceProviderOverride = null;
    }
}
#endif

/// <summary>
/// Represents the core module for dependency injection within the Unreal Engine,
/// facilitating advanced service provider configurations and runtime service management.
/// </summary>
/// <remarks>
/// This module serves as the central integration point for leveraging Microsoft.Extensions.DependencyInjection
/// within Unreal Engine. It provides mechanisms for managing dependency services, supporting
/// both customization and overriding of service providers for specific scenarios like game instances.
/// The module is implemented as a singleton and should be initialized during game startup.
/// </remarks>
[UsedImplicitly]
public sealed class FUnrealInjectModule : IModuleInterface
{
    private static FUnrealInjectModule? _instance;

    /// <summary>
    /// Provides access to the dependency injection service collection within the Unreal Engine integration.
    /// </summary>
    /// <remarks>
    /// The <c>Services</c> property represents the core collection of registered services used for dependency injection.
    /// It allows for registering, modifying, and retrieving service descriptors at runtime.
    /// The underlying implementation is based on the Microsoft.Extensions.DependencyInjection framework.
    /// This collection is primarily used to configure services required across various Unreal Engine modules.
    /// </remarks>
    public UnrealServiceCollection Services { get; } = [];
#if WITH_EDITOR
    internal IServiceProvider? GameInstanceServiceProviderOverride
    {
        get;
        set
        {
            if (field is IDisposable disposable)
            {
                disposable.Dispose();
            }

            field = value;
            OnGameInstanceServiceProviderChanged?.Invoke(field);
        }
    }

    internal event Action<IServiceProvider?>? OnGameInstanceServiceProviderChanged;
#endif

    private IServiceProviderFactory<object> _serviceProviderFactory =
        new ServiceProviderFactoryAdapter<IServiceCollection>(new DefaultServiceProviderFactory());

    /// <summary>
    /// Provides a singleton instance of the <c>FUnrealInjectModule</c> class.
    /// </summary>
    /// <remarks>
    /// The <c>Instance</c> property serves as the globally accessible point of entry for the UnrealInject module.
    /// It ensures that only one instance of the module is active and provides access to its functionality.
    /// Accessing the <c>Instance</c> property before the module is initialized will result in an <c>InvalidOperationException</c>.
    /// This property is essential for managing service registrations and configurations across different modules.
    /// </remarks>
    public static FUnrealInjectModule Instance =>
        _instance ?? throw new InvalidOperationException("The UnrealInject module is not initialized.");

    /// <inheritdoc />
    public void StartupModule()
    {
        _instance = this;
        Services.AddSingleton(typeof(IDeveloperSettings<>), typeof(DeveloperSettings<>));
    }

    /// <inheritdoc />
    public void ShutdownModule()
    {
        _instance = null;
    }

    /// <summary>
    /// Configures a custom service provider factory to be used with the Unreal Inject module.
    /// This allows for advanced customization of dependency injection behaviors and provider setups.
    /// </summary>
    /// <typeparam name="TBuilder">
    /// The type of the builder used by the custom service provider factory. This type must be non-nullable.
    /// </typeparam>
    /// <param name="factory">
    /// An instance of <see cref="IServiceProviderFactory{TBuilder}"/> that defines how service containers are built
    /// and converted into service providers.
    /// </param>
    /// <returns>
    /// A reference to the current <see cref="FUnrealInjectModule"/> instance, enabling method chaining for further configuration.
    /// </returns>
    public FUnrealInjectModule UseServiceProviderFactory<TBuilder>(IServiceProviderFactory<TBuilder> factory)
        where TBuilder : notnull
    {
        _serviceProviderFactory = new ServiceProviderFactoryAdapter<TBuilder>(factory);
        return this;
    }

    internal IServiceProvider BuildServiceProvider()
    {
#if WITH_EDITOR
        if (GameInstanceServiceProviderOverride is not null)
        {
            return GameInstanceServiceProviderOverride;
        }
#endif
        var containerBuilder = _serviceProviderFactory.CreateBuilder(Services);
        var provider = _serviceProviderFactory.CreateServiceProvider(containerBuilder);
        return provider;
    }

    private sealed class ServiceProviderFactoryAdapter<TBuilder>([ReadOnly] IServiceProviderFactory<TBuilder> factory)
        : IServiceProviderFactory<object>
        where TBuilder : notnull
    {
        public object CreateBuilder(IServiceCollection services)
        {
            return factory.CreateBuilder(services);
        }

        public IServiceProvider CreateServiceProvider(object containerBuilder)
        {
            return factory.CreateServiceProvider((TBuilder)containerBuilder);
        }
    }
}
