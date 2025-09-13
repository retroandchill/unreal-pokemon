using System.Runtime.CompilerServices;
using Microsoft.Extensions.DependencyInjection;
using UnrealSharp.Attributes;
using UnrealSharp.UnrealSharpCore;

namespace UnrealInject.Subsystems;

[UClass]
public sealed class UDependencyInjectionGameInstanceSubsystem
    : UCSGameInstanceSubsystem,
        IServiceProvider,
        IServiceScope
{
    private IServiceScope _serviceScope = null!;

    public IServiceProvider ServiceProvider => _serviceScope.ServiceProvider;

    protected override void Initialize(FSubsystemCollectionBaseRef collection)
    {
        SetServiceScope(FUnrealInjectModule.Instance.GameInstanceServiceProviderOverride);
    }

    private void SetServiceScope(IServiceProvider? serviceProviderOverride)
    {
#if WITH_EDITOR
        if (serviceProviderOverride is not null)
        {
            _serviceScope = serviceProviderOverride.CreateScope();
            FUnrealInjectModule.Instance.OnGameInstanceServiceProviderChanged += SetServiceScope;
            return;
        }
#endif
        var engineSubsystem = GetEngineSubsystem<UDependencyInjectionEngineSubsystem>();
        engineSubsystem.OnServiceProviderRebuilt += RebuildServiceProvider;

        _serviceScope = engineSubsystem.CreateScope();
    }

    protected override void Deinitialize()
    {
#if WITH_EDITOR
        FUnrealInjectModule.Instance.OnGameInstanceServiceProviderChanged -= SetServiceScope;
#endif
        var engineSubsystem = GetEngineSubsystem<UDependencyInjectionEngineSubsystem>();
        engineSubsystem.OnServiceProviderRebuilt -= RebuildServiceProvider;
        if (_serviceScope is IDisposable disposable)
        {
            disposable.Dispose();
        }
    }

    internal void RebuildServiceProvider(IServiceProvider provider)
    {
        if (_serviceScope is IDisposable disposable)
        {
            disposable.Dispose();
        }

        _serviceScope = provider.CreateScope();
    }

    public object? GetService(Type serviceType)
    {
        return _serviceScope.ServiceProvider.GetService(serviceType);
    }
}
