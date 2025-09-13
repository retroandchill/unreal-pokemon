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

    private readonly ServiceCollection _serviceCollection = [];
    private bool _servicesBuilt;
#if WITH_EDITOR
    private IServiceProvider? _gameInstanceServiceProviderOverride;
    internal IServiceProvider? GameInstanceServiceProviderOverride
    {
        get => _gameInstanceServiceProviderOverride;
        set
        {
            if (_gameInstanceServiceProviderOverride is IDisposable disposable)
            {
                disposable.Dispose();
            }
            _gameInstanceServiceProviderOverride = value;
            OnGameInstanceServiceProviderChanged?.Invoke(_gameInstanceServiceProviderOverride);
        }
    }
    internal event Action<IServiceProvider?>? OnGameInstanceServiceProviderChanged;
#endif

    public event Action<IServiceProvider>? OnServiceProviderRebuilt;

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
        _servicesBuilt = false;
        _instance = this;
    }

    public void ShutdownModule()
    {
        _servicesBuilt = false;
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

    public FUnrealInjectModule ConfigureServices(Action<IServiceCollection> configureDelegate)
    {
        configureDelegate(_serviceCollection);
        if (_servicesBuilt)
        {
            OnServiceProviderRebuilt?.Invoke(BuildServiceProvider());
        }
        return this;
    }

    internal IServiceProvider BuildServiceProvider()
    {
        var containerBuilder = _serviceProviderFactory.CreateBuilder(_serviceCollection);
        var provider = _serviceProviderFactory.CreateServiceProvider(containerBuilder);
        _servicesBuilt = true;
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
