using JetBrains.Annotations;
using Microsoft.Extensions.DependencyInjection;
using Retro.ReadOnlyParams.Annotations;
using UnrealSharp.Engine.Core.Modules;

namespace UnrealInject;

#if WITH_EDITOR
public sealed class GameInstanceServiceProviderOverride : IDisposable
{
    public GameInstanceServiceProviderOverride(IServiceProvider serviceProvider)
    {
        FUnrealInjectModule.Instance.GameInstanceServiceProviderOverride = serviceProvider;
    }

    public void Dispose()
    {
        FUnrealInjectModule.Instance.GameInstanceServiceProviderOverride = null;
    }
}
#endif

[UsedImplicitly]
public sealed class FUnrealInjectModule : IModuleInterface
{
    private static FUnrealInjectModule? _instance;

    private readonly ServiceCollection _services = [];
    public IServiceCollection Services => _services;
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

    public static FUnrealInjectModule Instance
    {
        get
        {
            if (_instance is null)
            {
                throw new InvalidOperationException("The UnrealInject module is not initialized.");
            }

            return _instance;
        }
    }

    public void StartupModule()
    {
        _instance = this;
    }

    public void ShutdownModule()
    {
        _instance = null;
    }

    public FUnrealInjectModule UseServiceProviderFactory<TBuilder>(
        IServiceProviderFactory<TBuilder> factory
    )
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
        var containerBuilder = _serviceProviderFactory.CreateBuilder(_services);
        var provider = _serviceProviderFactory.CreateServiceProvider(containerBuilder);
        return provider;
    }

    private sealed class ServiceProviderFactoryAdapter<TBuilder>(
        [ReadOnly] IServiceProviderFactory<TBuilder> factory
    ) : IServiceProviderFactory<object>
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
