using Microsoft.Extensions.DependencyInjection;
using UnrealSharp.Attributes;
using UnrealSharp.LevelEditor;
using UnrealSharp.UnrealSharpCore;

namespace UnrealInject.Subsystems;

[UClass]
public class UDependencyInjectionWorldSubsystem : UCSWorldSubsystem, IServiceProvider, IServiceScope
{
    private IServiceScope _serviceScope = null!;

    public IServiceProvider ServiceProvider => _serviceScope.ServiceProvider;

    protected override void Initialize(FSubsystemCollectionBaseRef collection)
    {
        #if WITH_EDITOR
        var levelEditorSubsystem = GetEditorSubsystem<ULevelEditorSubsystem>();
        if (!levelEditorSubsystem.IsInPlayInEditor()) return;
        #endif
        
        var gameInstanceSubsystem = GetGameInstanceSubsystem<UDependencyInjectionGameInstanceSubsystem>();
        _serviceScope = gameInstanceSubsystem.CreateScope();
    }

    protected override void Deinitialize()
    {
        if (_serviceScope is IDisposable disposable)
        {
            disposable.Dispose();
        }
    }

    public object? GetService(Type serviceType)
    {
        return ServiceProvider.GetService(serviceType);
    }
}